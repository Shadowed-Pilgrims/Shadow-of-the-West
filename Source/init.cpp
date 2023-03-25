/**
 * @file init.cpp
 *
 * Implementation of routines for initializing the environment, disable screen saver, load MPQ.
 */
#include <SDL.h>
#include <config.h>
#include <string>
#include <vector>

#if (defined(_WIN64) || defined(_WIN32)) && !defined(__UWP__) && !defined(NXDK)
#include <find_steam_game.h>
#endif

#include "DiabloUI/diabloui.h"
#include "engine/assets.hpp"
#include "engine/backbuffer_state.hpp"
#include "engine/dx.h"
#include "engine/events.hpp"
#include "hwcursor.hpp"
#include "options.h"
#include "pfile.h"
#include "utils/file_util.h"
#include "utils/language.h"
#include "utils/log.hpp"
#include "utils/paths.h"
#include "utils/str_split.hpp"
#include "utils/ui_fwd.h"
#include "utils/utf8.hpp"

#ifndef UNPACKED_MPQS
#include "mpq/mpq_reader.hpp"
#endif

#ifdef __vita__
// increase default allowed heap size on Vita
int _newlib_heap_size_user = 100 * 1024 * 1024;
#endif

namespace devilution {

/** True if the game is the current active window */
bool gbActive;

#ifdef UNPACKED_MPQS
std::optional<std::string> spawn_data_path;
std::optional<std::string> diabdat_data_path;
std::optional<std::string> hellfire_data_path;
std::optional<std::string> font_data_path;
std::optional<std::string> lang_data_path;
#else
std::optional<MpqArchive> spawn_mpq;
std::optional<MpqArchive> diabdat_mpq;
std::optional<MpqArchive> hellfire_mpq;
std::optional<MpqArchive> hfmonk_mpq;
std::optional<MpqArchive> hfbard_mpq;
std::optional<MpqArchive> hfbarb_mpq;
std::optional<MpqArchive> hfmusic_mpq;
std::optional<MpqArchive> hfvoice_mpq;
std::optional<MpqArchive> sotw_mpq;
std::optional<MpqArchive> lang_mpq;
std::optional<MpqArchive> font_mpq;
#endif

namespace {

#ifdef UNPACKED_MPQS
std::optional<std::string> FindUnpackedMpqData(const std::vector<std::string> &paths, string_view mpqName)
{
	std::string targetPath;
	for (const std::string &path : paths) {
		targetPath.clear();
		targetPath.reserve(path.size() + mpqName.size() + 1);
		targetPath.append(path).append(mpqName) += DirectorySeparator;
		if (FileExists(targetPath)) {
			LogVerbose("  Found unpacked MPQ directory: {}", targetPath);
			return targetPath;
		}
	}
	return std::nullopt;
}
#else
std::optional<MpqArchive> LoadMPQ(const std::vector<std::string> &paths, string_view mpqName)
{
	std::optional<MpqArchive> archive;
	std::string mpqAbsPath;
	std::int32_t error = 0;
	for (const auto &path : paths) {
		mpqAbsPath = path + mpqName.data();
		if ((archive = MpqArchive::Open(mpqAbsPath.c_str(), error))) {
			LogVerbose("  Found: {} in {}", mpqName, path);
			return archive;
		}
		if (error != 0) {
			LogError("Error {}: {}", MpqArchive::ErrorMessage(error), mpqAbsPath);
		}
	}
	if (error == 0) {
		LogVerbose("Missing: {}", mpqName);
	}

	return std::nullopt;
}
#endif

std::vector<std::string> GetMPQSearchPaths()
{
	std::vector<std::string> paths;
	paths.push_back(paths::BasePath());
	paths.push_back(paths::PrefPath());
	if (paths[0] == paths[1])
		paths.pop_back();
	paths.push_back(paths::ConfigPath());
	if (paths[0] == paths[1] || (paths.size() == 3 && (paths[0] == paths[2] || paths[1] == paths[2])))
		paths.pop_back();

#if defined(__unix__) && !defined(__ANDROID__)
	// `XDG_DATA_HOME` is usually the root path of `paths::PrefPath()`, so we only
	// add `XDG_DATA_DIRS`.
	const char *xdgDataDirs = std::getenv("XDG_DATA_DIRS");
	if (xdgDataDirs != nullptr) {
		for (const string_view path : SplitByChar(xdgDataDirs, ':')) {
			std::string fullPath(path);
			if (!path.empty() && path.back() != '/')
				fullPath += '/';
			fullPath.append("shadowed-pilgrims/shadow-of-the-west/");
			paths.push_back(std::move(fullPath));
		}
	} else {
		paths.emplace_back("/usr/local/share/shadowed-pilgrims/shadow-of-the-west/");
		paths.emplace_back("/usr/share/shadowed-pilgrims/shadow-of-the-west/");
	}
#elif defined(NXDK)
	paths.emplace_back("D:\\");
#elif (defined(_WIN64) || defined(_WIN32)) && !defined(__UWP__) && !defined(NXDK)
	char gogpath[_FSG_PATH_MAX];
	fsg_get_gog_game_path(gogpath, "1412601690");
	if (strlen(gogpath) > 0) {
		paths.emplace_back(std::string(gogpath) + "/");
		paths.emplace_back(std::string(gogpath) + "/hellfire/");
	}
#endif

	paths.emplace_back(""); // PWD

	if (SDL_LOG_PRIORITY_VERBOSE >= SDL_LogGetPriority(SDL_LOG_CATEGORY_APPLICATION)) {
		LogVerbose("Paths:\n    base: {}\n    pref: {}\n  config: {}\n  assets: {}",
		    paths::BasePath(), paths::PrefPath(), paths::ConfigPath(), paths::AssetsPath());

		std::string message;
		for (std::size_t i = 0; i < paths.size(); ++i) {
			message.append(fmt::format("\n{:6d}. '{}'", i + 1, paths[i]));
		}
		LogVerbose("MPQ search paths:{}", message);
	}

	return paths;
}

} // namespace

void init_cleanup()
{
	if (gbIsMultiplayer && gbRunGame) {
		pfile_write_hero(/*writeGameData=*/false);
		sfile_write_stash();
	}

#ifdef UNPACKED_MPQS
	lang_data_path = std::nullopt;
	font_data_path = std::nullopt;
	hellfire_data_path = std::nullopt;
	diabdat_data_path = std::nullopt;
	spawn_data_path = std::nullopt;
#else
	spawn_mpq = std::nullopt;
	diabdat_mpq = std::nullopt;
	hellfire_mpq = std::nullopt;
	hfmonk_mpq = std::nullopt;
	hfbard_mpq = std::nullopt;
	hfbarb_mpq = std::nullopt;
	hfmusic_mpq = std::nullopt;
	hfvoice_mpq = std::nullopt;
	lang_mpq = std::nullopt;
	font_mpq = std::nullopt;
	sotw_mpq = std::nullopt;
#endif

	NetClose();
}

void LoadCoreArchives()
{
	auto paths = GetMPQSearchPaths();

#ifdef UNPACKED_MPQS
	font_data_path = FindUnpackedMpqData(paths, "fonts");
#else // !UNPACKED_MPQS
#if !defined(__ANDROID__) && !defined(__APPLE__) && !defined(__3DS__) && !defined(__SWITCH__)
	// Load sotw.mpq first to get the font file for error messages
	sotw_mpq = LoadMPQ(paths, "sotw.mpq");
#endif
	font_mpq = LoadMPQ(paths, "fonts.mpq"); // Extra fonts
#endif
}

void LoadLanguageArchive()
{
#ifdef UNPACKED_MPQS
	lang_data_path = std::nullopt;
#else
	lang_mpq = std::nullopt;
#endif

	string_view code = GetLanguageCode();
	if (code != "en") {
		std::string langMpqName { code };
#ifdef UNPACKED_MPQS
		lang_data_path = FindUnpackedMpqData(GetMPQSearchPaths(), langMpqName);
#else
		langMpqName.append(".mpq");
		lang_mpq = LoadMPQ(GetMPQSearchPaths(), langMpqName);
#endif
	}
}

void LoadGameArchives()
{
	auto paths = GetMPQSearchPaths();
#ifdef UNPACKED_MPQS
	diabdat_data_path = FindUnpackedMpqData(paths, "diabdat");
	if (!HeadlessMode) {
		AssetRef ref = FindAsset("ui_art\\title.clx");
		if (!ref.ok()) {
			LogError("{}", SDL_GetError());
			InsertCDDlg("diabdat.mpq");
		}
	}
	hellfire_data_path = FindUnpackedMpqData(paths, "hellfire");
	if (!hellfire_data_path)
		InsertCDDlg("hellfire");

	const bool hasMonk = FileExists(*hellfire_data_path + "plrgfx/monk/mha/mhaas.clx");
	const bool hasMusic = FileExists(*hellfire_data_path + "music/dlvlf.wav")
	    || FileExists(*hellfire_data_path + "music/dlvlf.mp3");
	const bool hasVoice = FileExists(*hellfire_data_path + "sfx/hellfire/cowsut1.wav")
	    || FileExists(*hellfire_data_path + "sfx/hellfire/cowsut1.mp3");

	// Bard and barbarian are not currently supported in unpacked mode
	// because they use the same paths as rogue and warrior.
	gbBard = false;
	gbBarbarian = false;

	if (!hasMonk || !hasMusic || !hasVoice) {
		UiErrorOkDialog(_("Some Hellfire MPQs are missing"), _("Not all Hellfire MPQs were found.\nPlease copy all the hf*.mpq files."));
		diablo_quit(1);
	}
#else // !UNPACKED_MPQS
	diabdat_mpq = LoadMPQ(paths, "DIABDAT.MPQ");
	if (!diabdat_mpq) {
		// DIABDAT.MPQ is uppercase on the original CD and the GOG version.
		diabdat_mpq = LoadMPQ(paths, "diabdat.mpq");
	}

	if (!HeadlessMode) {
		AssetRef ref = FindAsset("ui_art\\title.pcx");
		if (!ref.ok()) {
			LogError("{}", SDL_GetError());
			InsertCDDlg("diabdat.mpq");
		}
	}

	hellfire_mpq = LoadMPQ(paths, "hellfire.mpq");
	if (!hellfire_mpq)
		InsertCDDlg("hellfire.mpq");

	hfmonk_mpq = LoadMPQ(paths, "hfmonk.mpq");
	hfbard_mpq = LoadMPQ(paths, "hfbard.mpq");
	if (hfbard_mpq)
		gbBard = true;
	hfbarb_mpq = LoadMPQ(paths, "hfbarb.mpq");
	if (hfbarb_mpq)
		gbBarbarian = true;
	hfmusic_mpq = LoadMPQ(paths, "hfmusic.mpq");
	hfvoice_mpq = LoadMPQ(paths, "hfvoice.mpq");

	if (!hfmonk_mpq || !hfmusic_mpq || !hfvoice_mpq) {
		UiErrorOkDialog(_("Some Hellfire MPQs are missing"), _("Not all Hellfire MPQs were found.\nPlease copy all the hf*.mpq files."));
		diablo_quit(1);
	}
#endif
}

void init_create_window()
{
	if (!SpawnWindow(PROJECT_NAME))
		app_fatal(_("Unable to create main window"));
	dx_init();
	gbActive = true;
#ifndef USE_SDL1
	SDL_DisableScreenSaver();
#endif
}

void MainWndProc(const SDL_Event &event)
{
#ifndef USE_SDL1
	if (event.type != SDL_WINDOWEVENT)
		return;
	switch (event.window.event) {
	case SDL_WINDOWEVENT_HIDDEN:
		gbActive = false;
		break;
	case SDL_WINDOWEVENT_SHOWN:
		gbActive = false;
		RedrawEverything();
		break;
	case SDL_WINDOWEVENT_EXPOSED:
		RedrawEverything();
		break;
	case SDL_WINDOWEVENT_SIZE_CHANGED:
		ReinitializeHardwareCursor();
		break;
	case SDL_WINDOWEVENT_LEAVE:
		sgbMouseDown = CLICK_NONE;
		LastMouseButtonAction = MouseActionType::None;
		RedrawEverything();
		break;
	case SDL_WINDOWEVENT_CLOSE:
		diablo_quit(0);
		break;
	case SDL_WINDOWEVENT_FOCUS_LOST:
		diablo_focus_pause();
		break;
	case SDL_WINDOWEVENT_FOCUS_GAINED:
		diablo_focus_unpause();
		break;
	default:
		LogVerbose("Unhandled SDL_WINDOWEVENT event: ", event.window.event);
		break;
	}
#else
	if (event.type != SDL_ACTIVEEVENT)
		return;
	if ((event.active.state & SDL_APPINPUTFOCUS) != 0) {
		if (event.active.gain == 0)
			diablo_focus_pause();
		else
			diablo_focus_unpause();
	}
#endif
}

} // namespace devilution
