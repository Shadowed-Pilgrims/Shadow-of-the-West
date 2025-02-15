/**
 * @file objdat.h
 *
 * Interface of all object data.
 */
#pragma once

#include <cstdint>

#include "levels/gendung.h"
#include "utils/enum_traits.h"

namespace devilution {

enum theme_id : int8_t {
	THEME_BARREL,
	THEME_SHRINE,
	THEME_MONSTPIT,
	THEME_SKELROOM,
	THEME_TREASURE,
	THEME_LIBRARY,
	THEME_TORTURE,
	THEME_BLOODFOUNTAIN,
	THEME_DECAPITATED,
	THEME_PURIFYINGFOUNTAIN,
	THEME_ARMORSTAND,
	THEME_GOATSHRINE,
	THEME_CAULDRON,
	THEME_MURKYFOUNTAIN,
	THEME_TEARFOUNTAIN,
	THEME_BRNCROSS,
	THEME_WEAPONRACK,
	THEME_NONE = -1,
};

enum object_graphic_id : int8_t {
	OFILE_L1BRAZ,
	OFILE_L1DOORS,
	OFILE_LEVER,
	OFILE_CHEST1,
	OFILE_CHEST2,
	OFILE_BANNER,
	OFILE_SKULPILE,
	OFILE_SKULFIRE,
	OFILE_SKULSTIK,
	OFILE_CRUXSK1,
	OFILE_CRUXSK2,
	OFILE_CRUXSK3,
	OFILE_BOOK1,
	OFILE_BOOK2,
	OFILE_ROCKSTAN,
	OFILE_ANGEL,
	OFILE_CHEST3,
	OFILE_BURNCROS,
	OFILE_CANDLE2,
	OFILE_NUDE2,
	OFILE_SWITCH4,
	OFILE_TNUDEM,
	OFILE_TNUDEW,
	OFILE_TSOUL,
	OFILE_L2DOORS,
	OFILE_WTORCH4,
	OFILE_WTORCH3,
	OFILE_SARC,
	OFILE_FLAME1,
	OFILE_PRSRPLT1,
	OFILE_TRAPHOLE,
	OFILE_MINIWATR,
	OFILE_WTORCH2,
	OFILE_WTORCH1,
	OFILE_BCASE,
	OFILE_BSHELF,
	OFILE_WEAPSTND,
	OFILE_BARREL,
	OFILE_BARRELEX,
	OFILE_LSHRINEG,
	OFILE_RSHRINEG,
	OFILE_BLOODFNT,
	OFILE_DECAP,
	OFILE_PEDISTL,
	OFILE_L3DOORS,
	OFILE_PFOUNTN,
	OFILE_ARMSTAND,
	OFILE_GOATSHRN,
	OFILE_CAULDREN,
	OFILE_MFOUNTN,
	OFILE_TFOUNTN,
	OFILE_ALTBOY,
	OFILE_MCIRL,
	OFILE_BKSLBRNT,
	OFILE_MUSHPTCH,
	OFILE_LZSTAND,
	OFILE_POD,
	OFILE_PODEX,
	OFILE_L5DOORS,
	OFILE_L5LEVER,
	OFILE_L5CANDLE,
	OFILE_L5SARC,
	OFILE_URN,
	OFILE_URNEX,
	OFILE_L5BOOKS,
	OFILE_S1DOORS,
	OFILE_STORCH2SE,
	OFILE_STORCH2SW,

	OFILE_LAST = OFILE_STORCH2SW,
	OFILE_NULL = -1,
};

enum _object_id : int8_t {
	OBJ_L1LIGHT,
	OBJ_L1LDOOR,
	OBJ_L1RDOOR,
	OBJ_SKFIRE,
	OBJ_LEVER,
	OBJ_CHEST1,
	OBJ_CHEST2,
	OBJ_CHEST3,
	OBJ_CANDLE1,
	OBJ_CANDLE2,
	OBJ_CANDLEO,
	OBJ_BANNERL,
	OBJ_BANNERM,
	OBJ_BANNERR,
	OBJ_SKPILE,
	OBJ_SKSTICK1,
	OBJ_SKSTICK2,
	OBJ_SKSTICK3,
	OBJ_SKSTICK4,
	OBJ_SKSTICK5,
	OBJ_CRUX1,
	OBJ_CRUX2,
	OBJ_CRUX3,
	OBJ_STAND,
	OBJ_ANGEL,
	OBJ_BOOK2L,
	OBJ_BCROSS,
	OBJ_NUDEW2R,
	OBJ_SWITCHSKL,
	OBJ_TNUDEM1,
	OBJ_TNUDEM2,
	OBJ_TNUDEM3,
	OBJ_TNUDEM4,
	OBJ_TNUDEW1,
	OBJ_TNUDEW2,
	OBJ_TNUDEW3,
	OBJ_TORTURE1,
	OBJ_TORTURE2,
	OBJ_TORTURE3,
	OBJ_TORTURE4,
	OBJ_TORTURE5,
	OBJ_BOOK2R,
	OBJ_L2LDOOR,
	OBJ_L2RDOOR,
	OBJ_TORCHL,
	OBJ_TORCHR,
	OBJ_TORCHL2,
	OBJ_TORCHR2,
	OBJ_SARC,
	OBJ_FLAMEHOLE,
	OBJ_FLAMELVR,
	OBJ_WATER,
	OBJ_BOOKLVR,
	OBJ_TRAPL,
	OBJ_TRAPR,
	OBJ_BOOKSHELF,
	OBJ_WEAPRACK,
	OBJ_BARREL,
	OBJ_BARRELEX,
	OBJ_SHRINEL,
	OBJ_SHRINER,
	OBJ_SKELBOOK,
	OBJ_BOOKCASEL,
	OBJ_BOOKCASER,
	OBJ_BOOKSTAND,
	OBJ_BOOKCANDLE,
	OBJ_BLOODFTN,
	OBJ_DECAP,
	OBJ_TCHEST1,
	OBJ_TCHEST2,
	OBJ_TCHEST3,
	OBJ_BLINDBOOK,
	OBJ_BLOODBOOK,
	OBJ_PEDESTAL,
	OBJ_L3LDOOR,
	OBJ_L3RDOOR,
	OBJ_PURIFYINGFTN,
	OBJ_ARMORSTAND,
	OBJ_ARMORSTANDN,
	OBJ_GOATSHRINE,
	OBJ_CAULDRON,
	OBJ_MURKYFTN,
	OBJ_TEARFTN,
	OBJ_ALTBOY,
	OBJ_MCIRCLE1,
	OBJ_MCIRCLE2,
	OBJ_STORYBOOK,
	OBJ_STORYCANDLE,
	OBJ_STEELTOME,
	OBJ_WARARMOR,
	OBJ_WARWEAP,
	OBJ_TBCROSS,
	OBJ_WEAPONRACK,
	OBJ_WEAPONRACKN,
	OBJ_MUSHPATCH,
	OBJ_LAZSTAND,
	OBJ_SLAINHERO,
	OBJ_SIGNCHEST,
	OBJ_BOOKSHELFR,
	OBJ_POD,
	OBJ_PODEX,
	OBJ_URN,
	OBJ_URNEX,
	OBJ_L5BOOKS,
	OBJ_L5CANDLE,
	OBJ_L5LDOOR,
	OBJ_L5RDOOR,
	OBJ_L5LEVER,
	OBJ_L5SARC,
	OBJ_S1LDOOR,
	OBJ_S1RDOOR,
	OBJ_STORCHL,
	OBJ_STORCHR,
	OBJ_STORCHL2,
	OBJ_STORCHR2,
	OBJ_NULL = -1,
};

enum quest_id : int8_t {
	Q_ROCK,
	Q_MUSHROOM,
	Q_GARBUD,
	Q_ZHAR,
	Q_VEIL,
	Q_DIABLO,
	Q_BUTCHER,
	Q_LTBANNER,
	Q_BLIND,
	Q_BLOOD,
	Q_ANVIL,
	Q_WARLORD,
	Q_SKELKING,
	Q_PWATER,
	Q_SCHAMB,
	Q_BETRAYER,
	Q_GRAVE,
	Q_FARMER,
	Q_GIRL,
	Q_TRADER,
	Q_DEFILER,
	Q_NAKRUL,
	Q_CORNSTN,
	Q_JERSEY,
	Q_INVALID = -1,
};

enum class ObjectDataFlags : uint8_t {
	Animated = 1U,
	Solid = 1U << 1,
	MissilesPassThrough = 1U << 2,
	Light = 1U << 3,
	Trap = 1U << 4,
	Breakable = 1U << 5,
};
use_enum_as_flags(ObjectDataFlags);

struct ObjectData {
	object_graphic_id ofindex;
	int8_t minlvl;
	int8_t maxlvl;
	dungeon_type olvltype;
	theme_id otheme;
	quest_id oquest;
	ObjectDataFlags flags;
	uint8_t animDelay; // Tick length of each frame in the current animation
	uint8_t animLen;   // Number of frames in current animation
	uint8_t animWidth;
	int8_t selFlag; // TODO Create enum

	[[nodiscard]] bool isAnimated() const
	{
		return HasAnyOf(flags, ObjectDataFlags::Animated);
	}

	[[nodiscard]] bool isSolid() const
	{
		return HasAnyOf(flags, ObjectDataFlags::Solid);
	}

	[[nodiscard]] bool missilesPassThrough() const
	{
		return HasAnyOf(flags, ObjectDataFlags::MissilesPassThrough);
	}

	[[nodiscard]] bool isLight() const
	{
		return HasAnyOf(flags, ObjectDataFlags::Light);
	}

	[[nodiscard]] bool isTrap() const
	{
		return HasAnyOf(flags, ObjectDataFlags::Trap);
	}

	[[nodiscard]] bool isBreakable() const
	{
		return HasAnyOf(flags, ObjectDataFlags::Breakable);
	}
};

extern const _object_id ObjTypeConv[];
extern const ObjectData AllObjects[115];
extern const char *const ObjMasterLoadList[];

} // namespace devilution
