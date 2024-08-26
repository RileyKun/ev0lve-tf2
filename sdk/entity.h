//
// Created by panzerfaust on 5/26/2020.
//

#ifndef EV0LVE_TF_ENTITY_H
#define EV0LVE_TF_ENTITY_H

namespace sdk {
    class entity_networkable;
    class entity_renderable;
    class entity_thinkable;
    class entity_client;
    class entity;

    enum class class_id : int {
        CTFWearableRazorback = 340,
        CTFWearableDemoShield = 337,
        CTFWearableLevelableItem = 339,
        CTFWearableCampaignItem = 336,
        CTFBaseRocket = 184,
        CTFWeaponBaseMerasmusGrenade = 324,
        CTFWeaponBaseMelee = 323,
        CTFWeaponBaseGun = 322,
        CTFWeaponBaseGrenadeProj = 321,
        CTFWeaponBase = 320,
        CTFWearableRobotArm = 341,
        CTFRobotArm = 286,
        CTFWrench = 343,
        CTFProjectile_ThrowableBreadMonster = 278,
        CTFProjectile_ThrowableBrick = 279,
        CTFProjectile_ThrowableRepel = 280,
        CTFProjectile_Throwable = 277,
        CTFThrowable = 318,
        CTFSyringeGun = 314,
        CTFKatana = 224,
        CTFSword = 313,
        CSniperDot = 117,
        CTFSniperRifleClassic = 307,
        CTFSniperRifleDecap = 308,
        CTFSniperRifle = 306,
        CTFChargedSMG = 196,
        CTFSMG = 305,
        CTFSlap = 304,
        CTFShovel = 303,
        CTFShotgunBuildingRescue = 302,
        CTFPEPBrawlerBlaster = 240,
        CTFSodaPopper = 309,
        CTFShotgun_Revenge = 300,
        CTFScatterGun = 296,
        CTFShotgun_Pyro = 299,
        CTFShotgun_HWG = 298,
        CTFShotgun_Soldier = 301,
        CTFShotgun = 297,
        CTFRocketPack = 295,
        CTFCrossbow = 200,
        CTFRocketLauncher_Mortar = 294,
        CTFRocketLauncher_AirStrike = 292,
        CTFRocketLauncher_DirectHit = 293,
        CTFRocketLauncher = 291,
        CTFRevolver = 285,
        CTFDRGPomson = 201,
        CTFRaygun = 283,
        CTFPistol_ScoutSecondary = 245,
        CTFPistol_ScoutPrimary = 244,
        CTFPistol_Scout = 243,
        CTFPistol = 242,
        CTFPipebombLauncher = 241,
        CTFWeaponPDA_Spy = 331,
        CTFWeaponPDA_Engineer_Destroy = 330,
        CTFWeaponPDA_Engineer_Build = 329,
        CTFWeaponPDAExpansion_Teleporter = 333,
        CTFWeaponPDAExpansion_Dispenser = 332,
        CTFWeaponPDA = 328,
        CTFParticleCannon = 238,
        CTFParachute_Secondary = 237,
        CTFParachute_Primary = 236,
        CTFParachute = 235,
        CTFMinigun = 233,
        CTFMedigunShield = 230,
        CWeaponMedigun = 351,
        CTFProjectile_MechanicalArmOrb = 262,
        CTFMechanicalArm = 229,
        CTFLunchBox_Drink = 228,
        CTFLunchBox = 227,
        CLaserDot = 78,
        CTFLaserPointer = 226,
        CTFKnife = 225,
        CTFGasManager = 211,
        CTFProjectile_JarGas = 260,
        CTFJarGas = 222,
        CTFProjectile_Cleaver = 253,
        CTFProjectile_JarMilk = 261,
        CTFProjectile_Jar = 259,
        CTFCleaver = 197,
        CTFJarMilk = 223,
        CTFJar = 221,
        CTFWeaponInvis = 327,
        CTFCannon = 195,
        CTFGrenadeLauncher = 215,
        CTFGrenadePipebombProjectile = 216,
        CTFGrapplingHook = 214,
        CTFFlareGun_Revenge = 209,
        CTFFlareGun = 208,
        CTFFlameRocket = 206,
        CTFFlameThrower = 207,
        CTFFists = 204,
        CTFFireAxe = 203,
        CTFWeaponFlameBall = 326,
        CTFCompoundBow = 199,
        CTFClub = 198,
        CTFBuffItem = 194,
        CTFStickBomb = 311,
        CTFBreakableSign = 193,
        CTFBottle = 191,
        CTFBreakableMelee = 192,
        CTFBonesaw = 189,
        CTFBall_Ornament = 181,
        CTFStunBall = 312,
        CTFBat_Giftwrap = 187,
        CTFBat_Wood = 188,
        CTFBat_Fish = 186,
        CTFBat = 185,
        CTFProjectile_EnergyRing = 255,
        CTFDroppedWeapon = 202,
        CTFWeaponSapper = 334,
        CTFWeaponBuilder = 325,
        C_TFWeaponBuilder = 0,
        CTFProjectile_Rocket = 263,
        CTFProjectile_Flare = 256,
        CTFProjectile_EnergyBall = 254,
        CTFProjectile_GrapplingHook = 257,
        CTFProjectile_HealingBolt = 258,
        CTFProjectile_Arrow = 251,
        CMannVsMachineStats = 80,
        CTFTankBoss = 315,
        CTFBaseBoss = 182,
        CBossAlpha = 0,
        NextBotCombatCharacter = 356,
        CTFProjectile_SpellKartBats = 267,
        CTFProjectile_SpellKartOrb = 268,
        CTFHellZap = 219,
        CTFProjectile_SpellLightningOrb = 269,
        CTFProjectile_SpellTransposeTeleport = 276,
        CTFProjectile_SpellMeteorShower = 270,
        CTFProjectile_SpellSpawnBoss = 273,
        CTFProjectile_SpellMirv = 271,
        CTFProjectile_SpellPumpkin = 272,
        CTFProjectile_SpellSpawnHorde = 274,
        CTFProjectile_SpellSpawnZombie = 275,
        CTFProjectile_SpellBats = 265,
        CTFProjectile_SpellFireball = 266,
        CTFSpellBook = 310,
        CHightower_TeleportVortex = 74,
        CTeleportVortex = 159,
        CZombie = 353,
        CMerasmusDancer = 83,
        CMerasmus = 82,
        CHeadlessHatman = 73,
        CEyeballBoss = 48,
        CTFBotHintEngineerNest = 190,
        CBotNPCMinion = 0,
        CBotNPC = 0,
        CPasstimeGun = 94,
        CTFViewModel = 319,
        CRobotDispenser = 111,
        CTFRobotDestruction_Robot = 287,
        CTFReviveMarker = 284,
        CTFPumpkinBomb = 281,
        CTFProjectile_BallOfFire = 252,
        CTFBaseProjectile = 183,
        CTFPointManager = 249,
        CBaseObjectUpgrade = 11,
        CTFRobotDestructionLogic = 290,
        CTFRobotDestruction_RobotGroup = 288,
        CTFRobotDestruction_RobotSpawn = 289,
        CTFPlayerDestructionLogic = 247,
        CPlayerDestructionDispenser = 101,
        CTFMinigameLogic = 232,
        CTFHalloweenMinigame_FallingPlatforms = 218,
        CTFHalloweenMinigame = 217,
        CTFMiniGame = 231,
        CTFPowerupBottle = 250,
        CTFItem = 220,
        CHalloweenSoulPack = 71,
        CTFGenericBomb = 212,
        CBonusRoundLogic = 23,
        CTFGameRulesProxy = 210,
        CTETFParticleEffect = 178,
        CTETFExplosion = 177,
        CTETFBlood = 176,
        CTFFlameManager = 205,
        CHalloweenGiftPickup = 69,
        CBonusDuckPickup = 21,
        CHalloweenPickup = 70,
        CCaptureFlagReturnIcon = 27,
        CCaptureFlag = 26,
        CBonusPack = 22,
        CTFTeam = 317,
        CTFTauntProp = 316,
        CTFPlayerResource = 248,
        CTFPlayer = 246,
        CTFRagdoll = 282,
        CTEPlayerAnimEvent = 164,
        CTFPasstimeLogic = 239,
        CPasstimeBall = 93,
        CTFObjectiveResource = 234,
        CTFGlow = 213,
        CTEFireBullets = 151,
        CTFBuffBanner = 0,
        CTFAmmoPack = 180,
        CObjectTeleporter = 89,
        CObjectSentrygun = 88,
        CTFProjectile_SentryRocket = 264,
        CObjectSapper = 87,
        CObjectCartDispenser = 85,
        CObjectDispenser = 86,
        CMonsterResource = 84,
        CFuncRespawnRoomVisualizer = 64,
        CFuncRespawnRoom = 63,
        CFuncPasstimeGoal = 61,
        CFuncForceField = 57,
        CCaptureZone = 28,
        CCurrencyPack = 31,
        CBaseObject = 10,
        CTestTraceline = 175,
        CTEWorldDecal = 179,
        CTESpriteSpray = 173,
        CTESprite = 172,
        CTESparks = 171,
        CTESmoke = 170,
        CTEShowLine = 168,
        CTEProjectedDecal = 166,
        CTEPlayerDecal = 165,
        CTEPhysicsProp = 163,
        CTEParticleSystem = 162,
        CTEMuzzleFlash = 161,
        CTELargeFunnel = 158,
        CTEKillPlayerAttachments = 157,
        CTEImpact = 156,
        CTEGlowSprite = 155,
        CTEShatterSurface = 167,
        CTEFootprintDecal = 153,
        CTEFizz = 152,
        CTEExplosion = 150,
        CTEEnergySplash = 149,
        CTEEffectDispatch = 148,
        CTEDynamicLight = 147,
        CTEDecal = 145,
        CTEClientProjectile = 144,
        CTEBubbleTrail = 143,
        CTEBubbles = 142,
        CTEBSPDecal = 141,
        CTEBreakModel = 140,
        CTEBloodStream = 139,
        CTEBloodSprite = 138,
        CTEBeamSpline = 137,
        CTEBeamRingPoint = 136,
        CTEBeamRing = 135,
        CTEBeamPoints = 134,
        CTEBeamLaser = 133,
        CTEBeamFollow = 132,
        CTEBeamEnts = 131,
        CTEBeamEntPoint = 130,
        CTEBaseBeam = 129,
        CTEArmorRicochet = 128,
        CTEMetalSparks = 160,
        CSteamJet = 122,
        CSmokeStack = 116,
        DustTrail = 354,
        CFireTrail = 50,
        SporeTrail = 361,
        SporeExplosion = 360,
        RocketTrail = 358,
        SmokeTrail = 359,
        CPropVehicleDriveable = 107,
        ParticleSmokeGrenade = 357,
        CParticleFire = 90,
        MovieExplosion = 355,
        CTEGaussExplosion = 154,
        CEnvQuadraticBeam = 43,
        CEmbers = 36,
        CEnvWind = 47,
        CPrecipitation = 106,
        CBaseTempEntity = 17,
        CWeaponIFMSteadyCam = 350,
        CWeaponIFMBaseCamera = 349,
        CWeaponIFMBase = 348,
        CTFWearableVM = 342,
        CTFWearable = 335,
        CTFWearableItem = 338,
        CEconWearable = 35,
        CBaseAttributableItem = 3,
        CEconEntity = 34,
        CHandleTest = 72,
        CTeamplayRoundBasedRulesProxy = 125,
        CTeamRoundTimer = 126,
        CSpriteTrail = 121,
        CSpriteOriented = 120,
        CSprite = 119,
        CRagdollPropAttached = 110,
        CRagdollProp = 109,
        CPoseController = 105,
        CGameRulesProxy = 68,
        CInfoLadderDismount = 75,
        CFuncLadder = 58,
        CEnvDetailController = 40,
        CWorld = 352,
        CWaterLODControl = 347,
        CWaterBullet = 346,
        CVoteController = 345,
        CVGuiScreen = 344,
        CPropJeep = 0,
        CPropVehicleChoreoGeneric = 0,
        CTest_ProxyToggle_Networkable = 174,
        CTesla = 169,
        CTeamTrainWatcher = 127,
        CBaseTeamObjectiveResource = 16,
        CTeam = 124,
        CSun = 123,
        CParticlePerformanceMonitor = 91,
        CSpotlightEnd = 118,
        CSlideshowDisplay = 115,
        CShadowControl = 114,
        CSceneEntity = 113,
        CRopeKeyframe = 112,
        CRagdollManager = 108,
        CPhysicsPropMultiplayer = 98,
        CPhysBoxMultiplayer = 96,
        CBasePropDoor = 15,
        CDynamicProp = 33,
        CPointCommentaryNode = 104,
        CPointCamera = 103,
        CPlayerResource = 102,
        CPlasma = 100,
        CPhysMagnet = 99,
        CPhysicsProp = 97,
        CPhysBox = 95,
        CParticleSystem = 92,
        CMaterialModifyControl = 81,
        CLightGlow = 79,
        CInfoOverlayAccessor = 77,
        CFuncTrackTrain = 67,
        CFuncSmokeVolume = 66,
        CFuncRotating = 65,
        CFuncReflectiveGlass = 62,
        CFuncOccluder = 60,
        CFuncMonitor = 59,
        CFunc_LOD = 54,
        CTEDust = 146,
        CFunc_Dust = 53,
        CFuncConveyor = 56,
        CBreakableSurface = 25,
        CFuncAreaPortalWindow = 55,
        CFish = 51,
        CEntityFlame = 38,
        CFireSmoke = 49,
        CEnvTonemapController = 46,
        CEnvScreenEffect = 44,
        CEnvScreenOverlay = 45,
        CEnvProjectedTexture = 42,
        CEnvParticleScript = 41,
        CFogController = 52,
        CEntityParticleTrail = 39,
        CEntityDissolve = 37,
        CDynamicLight = 32,
        CColorCorrectionVolume = 30,
        CColorCorrection = 29,
        CBreakableProp = 24,
        CBasePlayer = 13,
        CBaseFlex = 8,
        CBaseEntity = 7,
        CBaseDoor = 6,
        CBaseCombatCharacter = 4,
        CBaseAnimatingOverlay = 2,
        CBoneFollower = 20,
        CBaseAnimating = 1,
        CInfoLightingRelative = 76,
        CAI_BaseNPC = 0,
        CBeam = 19,
        CBaseViewModel = 18,
        CBaseProjectile = 14,
        CBaseParticleEntity = 12,
        CBaseGrenade = 9,
        CBaseCombatWeapon = 5,
    };

    enum life_state : char {
        ls_alive,
        ls_dying,
        ls_dead,
        ls_respawnable,
        ls_discard_body
    };

    enum  entity_flag : uint32_t {
        fl_on_ground       = 1U << 0U,
        fl_ducking         = 1U << 1U,
        fl_water_jump      = 1U << 3U,
        fl_on_train        = 1U << 4U,
        fl_in_train        = 1U << 5U,
        fl_frozen          = 1U << 6U,
        fl_at_controls     = 1U << 7U,
        fl_client          = 1U << 8U,
        fl_fake_client     = 1U << 9U,
        fl_in_water        = 1U << 10U,
        fl_fly             = 1U << 11U,
        fl_swim            = 1U << 12U,
        fl_conveyor        = 1U << 13U,
        fl_npc             = 1U << 14U,
        fl_god_mode        = 1U << 15U,
        fl_no_target       = 1U << 16U,
        fl_aim_target      = 1U << 17U,
        fl_partial_ground  = 1U << 18U,
        fl_static_prop     = 1U << 19U,
        fl_graphed         = 1U << 20U,
        fl_grenade         = 1U << 21U,
        fl_step_movement   = 1U << 22U,
        fl_dont_touch      = 1U << 23U,
        fl_base_velocity   = 1U << 24U,
        fl_world_brush     = 1U << 25U,
        fl_object          = 1U << 26U,
        fl_kill_me         = 1U << 27U,
        fl_on_fire         = 1U << 28U,
        fl_dissolving      = 1U << 29U,
        fl_trans_ragdoll   = 1U << 30U,
        fl_unblockable     = 1U << 31U,
    };

    enum move_type {
        mt_none		=0,
        mt_isometric,
        mt_walk,
        mt_step,
        mt_fly,
        mt_fly_gravity,
        mt_v_physics,
        mt_push,
        mt_no_clip,
        mt_ladder,
        mt_observer,
        mt_custom,
        mt_last		= mt_custom,
        mt_max_bits	= 4
    };

    class entity_handle {
    public:
        virtual			~entity_handle() = 0;
        virtual void	set_handle(const entity_handle&) = 0;
        virtual const	handle& get_handle() = 0;
    };

    class entity_unknown : public entity_handle {
    public:
        virtual collideable*		get_collideable() = 0;
        virtual entity_networkable* get_networkable() = 0;
        virtual entity_renderable*	get_renderable() = 0;
        virtual entity_client*		get_client_entity() = 0;
        virtual entity*			    get_entity() = 0;
        virtual entity_thinkable*	get_thinkable() = 0;
    };

    class entity_renderable {
    public:
        virtual entity_unknown*		get_unknown() = 0;
        virtual const vec3&		    get_render_origin() = 0;
        virtual const vec3&		    get_render_angles() = 0;
        virtual bool				is_transparent() = 0;
        virtual bool				uses_p2_texture() = 0;
        virtual bool				uses_full_texture() = 0;
        virtual uint16_t	        get_shadow_handle() = 0;
        virtual uint16_t&           render_handle() = 0;
        virtual void                unused() = 0;
        virtual const model*        get_model() = 0;
        virtual int					draw_model(int) = 0;
        virtual int		            get_body() = 0;
        virtual void	            compute_fx_blend() = 0;
        virtual int		            get_fx_blend() = 0;
        virtual void	            get_color_mod(float*) = 0;
        virtual bool	            lod_test() = 0;
        virtual bool	            setup_bones(matrix3x4*, int, int, float) = 0;
        virtual void	            setup_weights(const matrix3x4*, int, float*, float*) = 0;
        virtual void	            do_anim_events() = 0;
        virtual void*               get_pvs_notify_interface() = 0;
        virtual void	            get_render_bounds(vec3&, vec3&) = 0;
        virtual void	            get_render_bounds_ws(vec3&, vec3&) = 0;
        virtual void	            get_shadow_render_bounds(vec3&, vec3&, int) = 0;
        virtual bool	            should_recv_textures(int) = 0;
        virtual bool	            get_shadow_cast_dist(float*, int) = 0;
        virtual bool	            get_shadow_cast_dir(vec3*, int) = 0;
        virtual bool	            is_shadow_dirty() = 0;
        virtual void	            mark_shadow_dirty(bool) = 0;
        virtual entity_renderable*  get_shadow_parent() = 0;
        virtual entity_renderable*  first_shadow_child() = 0;
        virtual entity_renderable*  next_shadow_peer() = 0;
        virtual int                 shadow_cast_type() = 0;
        virtual void                create_model_instance() = 0;
        virtual uint16_t            get_model_stance() = 0;
        virtual const matrix3x4&    renderable_to_wt() = 0;
        virtual int                 lookup_attachment(const char*) = 0;
        virtual	bool                get_attachment(int, vec3&, ang&) = 0;
        virtual bool                get_attachment(int, matrix3x4&) = 0;
        virtual float*              get_render_clip_pane() = 0;
        virtual int		            get_skin() = 0;
        virtual bool	            is_two_pass() = 0;
        virtual void	            on_threaded_draw_setup() = 0;
        virtual bool	            uses_flex_delayed_weights() = 0;
        virtual void	            record_tool_message() = 0;
        virtual bool	            ignores_z_buffer() = 0;
    };

    class entity_networkable {
    public:
        virtual entity_unknown*	get_unknown() = 0;
        virtual void			release() = 0;
        virtual client_class*	get_client_class() = 0;
        virtual void			notify_should_transmit(void*) = 0;
        virtual void			on_pre_data_changed(void*) = 0;
        virtual void			on_data_changed(void*) = 0;
        virtual void			pre_data_update(void*) = 0;
        virtual void			post_data_update(void*) = 0;
        virtual bool			is_dormant() = 0;
        virtual int				index() = 0;
        virtual void			receive_message(int, void*) = 0;
        virtual void*			get_dt_base() = 0;
        virtual void			set_destroyed_on_recreate() = 0;
        virtual void			on_data_unchanged_in_pvs() = 0;
    };

    class entity_thinkable {
    public:
        virtual entity_unknown*	get_unknown() = 0;
        virtual void		    think() = 0;
        virtual void*		    get_think_handle() = 0;
        virtual void		    set_think_handle(void*) = 0;
        virtual void		    release() = 0;
    };

    class entity_client : public entity_unknown, public entity_renderable, public entity_networkable, public entity_thinkable {
    public:
        virtual void			release() = 0;
        virtual const vec3&		get_abs_origin() = 0;
        virtual const ang&		get_abs_angles() = 0;
        virtual void*			get_mouth() = 0;
        virtual bool			get_sound_thing(void*) = 0;
    };

    class weapon_data_t
    {
    public:
        int32_t damage; //0x0000
        int32_t bullets_per_shot; //0x0004
        float range; //0x0008
        float spread; //0x000C
        float punch_angle; //0x0010
        float fire_time_delay; //0x0014
        float time_idle; //0x0018
        float time_idle_empty; //0x001C
        float time_reload_start; //0x0020
        float time_reload; //0x0024
        bool draw_crosshair; //0x0028
        char pad_0029[3]; //0x0029
        int32_t projectile; //0x002C
        int32_t ammo_per_shot; //0x0030
        float projectile_speed; //0x0034
        float smack_delay; //0x0038
        bool use_rapid_fire_crits; //0x003C
        char pad_003D[3]; //0x003D
    }; //Size: 0x0040

    class weapon_data {
    public:
        void *vtable; //0x0000
        bool parsed_script; //0x0004
        bool loaded_hud_elements; //0x0005
        char class_name[80]; //0x0006
        char print_name[80]; //0x0056
        char view_model[80]; //0x00A6
        char world_model[80]; //0x00F6
        char anim_prefix[16]; //0x0146
        int32_t slot; //0x0156
        int32_t position; //0x015A
        // char pad_015E[2]; //0x015E
        int32_t max_clip; //0x0160
        int32_t max_clip_sec; //0x0164
        int32_t def_clip; //0x0168
        int32_t def_clip_sec; //0x016C
        int32_t weight; //0x0170
        int32_t rumble_effect; //0x0174
        bool auto_switch_to; //0x0178
        bool auto_switch_from; //0x0179
        char pad_017A[2]; //0x017A
        uint32_t flags; //0x017C
        char primary_ammo_name[32]; //0x0180
        char secondary_ammo_name[32]; //0x01A0
        char shoot_sounds[16][80]; //0x01C0
        int32_t ammo_type; //0x06C0
        int32_t ammo_type_sec; //0x06C4
        bool is_melee; //0x06C8
        bool built_right_handed; //0x06C9
        bool allow_flipping; //0x06CA
        char pad_06CB[1]; //0x06CB
        int32_t sprite_count; //0x06CC
        void *icon_active; //0x06D0
        void *icon_inactive; //0x06D4
        void *icon_ammo; //0x06D8
        void *icon_ammo_sec; //0x06DC
        void *icon_crosshair; //0x06E0
        void *icon_autoaim; //0x06E4
        void *icon_zoomed_crosshair; //0x06E8
        void *icon_zoomed_autoaim; //0x06EC
        void *icon_small; //0x06F0
        bool show_usage_hint; //0x06F4
        char pad_06F5[3]; //0x06F5
        class weapon_data_t data[2]; //0x06F8
        bool is_grenade; //0x0778
        char pad_0779[7]; //0x0779
        float damage_radius; //0x0780
        float primer_time; //0x0784
        bool lower_weapon; //0x0788
        bool suppress_nade_timer; //0x0789
        bool has_team_skins_vm; //0x078A
        bool has_team_skins_wm; //0x078B
        char muzzle_flash_model[128]; //0x078C
        float muzzle_flash_duration; //0x080C
        char muzzle_flash_particle[128]; //0x0810
        char tracer_effect[128]; //0x0890
        bool do_instant_eject_brass; //0x0910
        char brass_model[128]; //0x0911
        char explosion_sound[128]; //0x0991
        char explosion_effect[128]; //0x0A11
        char explosion_player_effect[128]; //0x0A91
        char explosion_water_effect[128]; //0x0B11
        bool dont_drop; //0x0B91
    };

    class fire_bullets_info {
    public:
        int shots{};
        vec3 src;
        vec3 dir_shooting;
        vec3 spread;
        float distance{};
        int ammo_type{};
        int tracer_freq{};
        float damage{};
        int player_damage{};
        int flags{};
        float damage_force_scale{};
        entity* attacker{};
        entity* additional_ignore_ent{};
        bool primary_attack{};
        bool use_server_random_seed{};
    };

    enum player_classes {
        pc_scout = 1,
        pc_sniper,
        pc_soldier,
        pc_demo_man,
        pc_medic,
        pc_heavy,
        pc_fire,
        pc_spy,
        pc_engineer
    };

    enum tf_cond : uint32_t {
        tf_cond_invalid = 0xFFFFFFFFU,
        tf_cond_aiming = 0U,
        tf_cond_zoomed,
        tf_cond_disguising,
        tf_cond_disguised,
        tf_cond_stealthed,
        tf_cond_invulnerable,
        tf_cond_teleported,
        tf_cond_taunting,
        tf_cond_invulnerable_wearingoff,
        tf_cond_stealthed_blink,
        tf_cond_selected_to_teleport,
        tf_cond_critboosted,
        tf_cond_tmpdamagebonus,
        tf_cond_feign_death,
        tf_cond_phase,
        tf_cond_stunned,
        tf_cond_offensebuff,
        tf_cond_shield_charge,
        tf_cond_demo_buff,
        tf_cond_energy_buff,
        tf_cond_radiusheal,
        tf_cond_health_buff,
        tf_cond_burning,
        tf_cond_health_overhealed,
        tf_cond_urine,
        tf_cond_bleeding,
        tf_cond_defensebuff,
        tf_cond_mad_milk,
        tf_cond_megaheal,
        tf_cond_regenondamagebuff,
        tf_cond_markedfordeath,
        tf_cond_nohealingdamagebuff,
        tf_cond_speed_boost,
        tf_cond_critboosted_pumpkin,
        tf_cond_critboosted_user_buff,
        tf_cond_critboosted_demo_charge,
        tf_cond_sodapopper_hype,
        tf_cond_critboosted_first_blood,
        tf_cond_critboosted_bonus_time,
        tf_cond_critboosted_ctf_capture,
        tf_cond_critboosted_on_kill,
        tf_cond_cannot_switch_from_melee,
        tf_cond_defensebuff_no_crit_block,
        tf_cond_reprogrammed,
        tf_cond_critboosted_rage_buff,
        tf_cond_defensebuff_high,
        tf_cond_snipercharge_rage_buff,
        tf_cond_disguise_wearingoff,
        tf_cond_markedfordeath_silent,
        tf_cond_disguised_as_dispenser,
        tf_cond_sapped,
        tf_cond_invulnerable_hide_unless_damaged,
        tf_cond_invulnerable_user_buff,
        tf_cond_halloween_bomb_head,
        tf_cond_halloween_thriller,
        tf_cond_radiusheal_on_damage,
        tf_cond_critboosted_card_effect,
        tf_cond_invulnerable_card_effect,
        tf_cond_medigun_uber_bullet_resist,
        tf_cond_medigun_uber_blast_resist,
        tf_cond_medigun_uber_fire_resist,
        tf_cond_medigun_small_bullet_resist,
        tf_cond_medigun_small_blast_resist,
        tf_cond_medigun_small_fire_resist,
        tf_cond_stealthed_user_buff,
        tf_cond_medigun_debuff,
        tf_cond_stealthed_user_buff_fading,
        tf_cond_bullet_immune,
        tf_cond_blast_immune,
        tf_cond_fire_immune,
        tf_cond_prevent_death,
        tf_cond_mvm_bot_stun_radiowave,
        tf_cond_halloween_speed_boost,
        tf_cond_halloween_quick_heal,
        tf_cond_halloween_giant,
        tf_cond_halloween_tiny,
        tf_cond_halloween_in_hell,
        tf_cond_halloween_ghost_mode,
        tf_cond_minicritboosted_on_kill,
        tf_cond_obscured_smoke,
        tf_cond_parachute_deployed,
        tf_cond_blastjumping,
        tf_cond_halloween_kart,
        tf_cond_halloween_kart_dash,
        tf_cond_balloon_head,
        tf_cond_melee_only,
        tf_cond_swimming_curse,
        tf_cond_freeze_input,
        tf_cond_halloween_kart_cage,
        tf_cond_donotuse_0,
        tf_cond_rune_strength,
        tf_cond_rune_haste,
        tf_cond_rune_regen,
        tf_cond_rune_resist,
        tf_cond_rune_vampire,
        tf_cond_rune_reflect,
        tf_cond_rune_precision,
        tf_cond_rune_agility,
        tf_cond_grapplinghook,
        tf_cond_grapplinghook_safefall,
        tf_cond_grapplinghook_latched,
        tf_cond_grapplinghook_bleeding,
        tf_cond_afterburn_immune,
        tf_cond_rune_knockout,
        tf_cond_rune_imbalance,
        tf_cond_critboosted_rune_temp,
        tf_cond_passtime_interception,
        tf_cond_swimming_no_effects,
        tf_cond_purgatory,
        tf_cond_rune_king,
        tf_cond_rune_plague,
        tf_cond_rune_supernova,
        tf_cond_plague,
        tf_cond_king_buffed,
        tf_cond_team_glows,
        tf_cond_knocked_into_air,
        tf_cond_competitive_winner,
        tf_cond_competitive_loser,
        tf_cond_healing_debuff,
        tf_cond_passtime_penalty_debuff,
        tf_cond_grappled_to_player,
        tf_cond_grappled_by_player,
        tf_cond_last
    };

    enum hitbox {
        hb_head = 0,
        hb_pelvis = 1,
        hb_spine_0 = 2,
        hb_spine_1 = 3,
        hb_spine_2 = 4,
        hb_spine_3 = 5,
        hb_upperarm_l = 6,
        hb_lowerarm_l = 7,
        hb_hand_l = 8,
        hb_upperarm_r = 9,
        hb_lowerarm_r = 10,
        hb_hand_r = 11,
        hb_hip_l = 12,
        hb_knee_l = 13,
        hb_foot_l = 14,
        hb_hip_r = 15,
        hb_knee_r = 16,
        hb_foot_r = 17,
    };

    enum weapon_type
    {
        tf_weapon_none = 0,
        tf_weapon_bat,
        tf_weapon_bat_wood,
        tf_weapon_bottle,
        tf_weapon_fireaxe,
        tf_weapon_club,
        tf_weapon_crowbar,
        tf_weapon_knife,
        tf_weapon_fists,
        tf_weapon_shovel,
        tf_weapon_wrench,
        tf_weapon_bonesaw,
        tf_weapon_shotgun_primary,
        tf_weapon_shotgun_soldier,
        tf_weapon_shotgun_hwg,
        tf_weapon_shotgun_pyro,
        tf_weapon_scattergun,
        tf_weapon_sniperrifle,
        tf_weapon_minigun,
        tf_weapon_smg,
        tf_weapon_syringegun_medic,
        tf_weapon_tranq,
        tf_weapon_rocketlauncher,
        tf_weapon_grenadelauncher,
        tf_weapon_pipebomblauncher,
        tf_weapon_flamethrower,
        tf_weapon_grenade_normal,
        tf_weapon_grenade_concussion,
        tf_weapon_grenade_nail,
        tf_weapon_grenade_mirv,
        tf_weapon_grenade_mirv_demoman,
        tf_weapon_grenade_napalm,
        tf_weapon_grenade_gas,
        tf_weapon_grenade_emp,
        tf_weapon_grenade_caltrop,
        tf_weapon_grenade_pipebomb,
        tf_weapon_grenade_smoke_bomb,
        tf_weapon_grenade_heal,
        tf_weapon_grenade_stunball,
        tf_weapon_grenade_jar,
        tf_weapon_grenade_jar_milk,
        tf_weapon_pistol,
        tf_weapon_pistol_scout,
        tf_weapon_revolver,
        tf_weapon_nailgun,
        tf_weapon_pda,
        tf_weapon_pda_engineer_build,
        tf_weapon_pda_engineer_destroy,
        tf_weapon_pda_spy,
        tf_weapon_builder,
        tf_weapon_medigun,
        tf_weapon_grenade_mirvbomb,
        tf_weapon_flamethrower_rocket,
        tf_weapon_grenade_demoman,
        tf_weapon_sentry_bullet,
        tf_weapon_sentry_rocket,
        tf_weapon_dispenser,
        tf_weapon_invis,
        tf_weapon_flaregun,
        tf_weapon_lunchbox,
        tf_weapon_jar,
        tf_weapon_compound_bow,
        tf_weapon_buff_item,
        tf_weapon_pumpkin_bomb,
        tf_weapon_sword,
        tf_weapon_rocketlauncher_directhit,
        tf_weapon_lifeline,
        tf_weapon_laser_pointer,
        tf_weapon_dispenser_gun,
        tf_weapon_sentry_revenge,
        tf_weapon_jar_milk,
        tf_weapon_handgun_scout_primary,
        tf_weapon_bat_fish,
        tf_weapon_crossbow,
        tf_weapon_stickbomb,
        tf_weapon_handgun_scout_secondary,
        tf_weapon_soda_popper,
        tf_weapon_sniperrifle_decap,
        tf_weapon_raygun,
        tf_weapon_particle_cannon,
        tf_weapon_mechanical_arm,
        tf_weapon_drg_pomson,
        tf_weapon_bat_giftwrap,
        tf_weapon_grenade_ornament_ball,
        tf_weapon_flaregun_revenge,
        tf_weapon_pep_brawler_blaster,
        tf_weapon_cleaver,
        tf_weapon_grenade_cleaver,
        tf_weapon_sticky_ball_launcher,
        tf_weapon_grenade_sticky_ball,
        tf_weapon_shotgun_building_rescue,
        tf_weapon_cannon,
        tf_weapon_throwable,
        tf_weapon_grenade_throwable,
        tf_weapon_pda_spy_build,
        tf_weapon_grenade_waterballoon,
        tf_weapon_harvester_saw,
        tf_weapon_spellbook,
        tf_weapon_spellbook_projectile,
        tf_weapon_sniperrifle_classic,
        tf_weapon_parachute,
        tf_weapon_grapplinghook,
        tf_weapon_passtime_gun,
        tf_weapon_sniperrifle_revolver,
        tf_weapon_charged_smg,
        tf_weapon_count
    };

    class cond_vars {
    public:
        cond_vars(uint32_t& cond, uint32_t& cond_ex, uint32_t& cond_ex_2, uint32_t& cond_ex_3, tf_cond c) {
            if (c >= 96) {
                cond_var = &cond_ex_3;
                cond_bit = c - 96;
            } else if (c >= 64) {
                cond_var = &cond_ex_2;
                cond_bit = c - 64;
            } else if (c >= 32) {
                cond_var = &cond_ex;
                cond_bit = c - 32;
            } else {
                cond_var = &cond;
                cond_bit = c;
            }
        }

        inline uint32_t& get_cond_var() {
            return *cond_var;
        }

        inline uint32_t get_cond_bit() {
            return 1U << cond_bit;
        }
    private:
        uint32_t* cond_var;
        uint32_t cond_bit;
    };

    class player_anim_state {
    public:
        void update(float pitch, float yaw);

        char pad_0000[28]; //0x0000
        void *player; //0x001C
        ang render_angle; //0x0020
        bool pose_parameter_init; //0x002C
        char pad_002D[3]; //0x002D
        int32_t pp_move_x; //0x0030
        int32_t pp_move_y; //0x0034
        int32_t pp_aim_yaw; //0x0038
        int32_t pp_aim_pitch; //0x003C
        int32_t pp_body_height; //0x0040
        int32_t pp_move_yaw; //0x0044
        int32_t pp_move_scale; //0x0048
        float pp_estimate_yaw; //0x004C
        float pp_last_aim_turn_time; //0x0050
        float dbg_speed; //0x0054
        float dbg_aim_pitch; //0x0058
        float dbg_aim_yaw; //0x005C
        float dbg_body_height; //0x0060
        float dbg_move_yaw_p; //0x0064
        float dbg_move_yaw_y; //0x0068
        bool current_feet_yaw_initialized; //0x006C
        char pad_006D[3]; //0x006D
        float last_animation_state_clear_time; //0x0070
        float eye_yaw; //0x0074
        float eye_pitch; //0x0078
        float goal_feet_yaw; //0x007C
        float current_feet_yaw; //0x0080
        float last_aim_turn_time; //0x0084
        float walk_speed; //0x0088
        float run_speed; //0x008C
        float sprint_speed; //0x0090
        float body_yaw_rate; //0x0094
        bool is_jumping; //0x0098
        char pad_0099[3]; //0x0099
        float jump_start_time; //0x009C
        bool first_jump_frame; //0x00A0
        bool in_swim; //0x00A1
        bool first_swim_frame; //0x00A2
        bool is_dying; //0x00A3
        bool first_dying_frame; //0x00A4
        char pad_00A5[3]; //0x00A5
        int32_t current_main_sequence_activity; //0x00A8
        int32_t specific_main_sequence; //0x00AC
        int32_t active_weapon; //0x00B0
        float last_ground_speed_update_time; //0x00B4
        char pad_00B8[48]; //0x00B8
        float max_ground_speed; //0x00E8
        int32_t movement_sequence; //0x00EC
        int32_t leg_anim_type; //0x00F0
        void *tf_player; //0x00F4
        bool in_air_walk; //0x00F8
        char pad_00F9[3]; //0x00F9
        float hold_deployed_pose_until; //0x00FC
        float taunt_move_x; //0x0100
        float taunt_move_y; //0x0104
        float vehicle_lean_vel; //0x0108
        float vehicle_lean_pos; //0x010C
        vec3 smoothed_up; //0x0110
    };

    class entity : public entity_client {
    public:
        inline bool is_player() {
            if (auto c = get_client_class())
                return c->class_id == (int)class_id::CTFPlayer;
            return false;
        }

        inline bool is_object() {
            if (auto c = get_client_class())
                return
                    c->class_id == (int)class_id::CObjectCartDispenser ||
                    c->class_id == (int)class_id::CObjectDispenser ||
                    c->class_id == (int)class_id::CObjectSapper ||
                    c->class_id == (int)class_id::CObjectSentrygun ||
                    c->class_id == (int)class_id::CObjectTeleporter;
            return false;
        }

        inline bool is_weapon() {
            return hack::virtual_func<bool(__thiscall*)(void*)>(this, 137)(this);
        }

        inline int max_health() {
            return hack::virtual_func<int(__thiscall*)(void*)>(this, 107)(this);
        }

        inline int weapon_id() {
            return hack::virtual_func<int(__thiscall*)(void*)>(this, 377)(this);
        }

        inline void thirdperson_switch(bool v) {
            hack::virtual_func<void(__thiscall*)(void*, bool)>(this, 252)(this, v);
        }

        inline bool can_attack() {
            return hack::virtual_func<bool(__thiscall*)(void*)>(this, 409)(this);
        }

        inline bool in_cond(tf_cond c) {
            if (c < 32 && (cond_bits() & (1U << c)))
                return true;

            cond_vars vars(player_cond(), player_cond_ex(), player_cond_ex_2(), player_cond_ex_3(), c);
            return (vars.get_cond_var() & vars.get_cond_bit());
        }

        void set_abs_origin(const vec3& o);
        void set_abs_angles(const ang& a);

        weapon_data* get_weapon_data();
        vec3 get_hitbox_pos(hitbox hb, matrix3x4* bones);

        inline vec3 view_offset() {
            auto x = netvars[JOAAT("DT_BasePlayer")][JOAAT("localdata")][JOAAT("m_vecViewOffset[0]")];
            auto y = netvars[JOAAT("DT_BasePlayer")][JOAAT("localdata")][JOAAT("m_vecViewOffset[1]")];
            auto z = netvars[JOAAT("DT_BasePlayer")][JOAAT("localdata")][JOAAT("m_vecViewOffset[2]")];

            return vec3(x.get<float>(this), y.get<float>(this), z.get<float>(this));
        }

        inline uint32_t& flags() {
            return netvars[JOAAT("DT_BasePlayer")][JOAAT("m_fFlags")].get<uint32_t>(this);
        }

        inline char& life_state() {
            return netvars[JOAAT("DT_BasePlayer")][JOAAT("m_lifeState")].get<char>(this);
        }

        inline vec3& velocity() {
            return netvars[JOAAT("DT_BasePlayer")][JOAAT("localdata")][JOAAT("m_vecVelocity[0]")].get<vec3>(this);
        }

        inline int& health() {
            return netvars[JOAAT("DT_BasePlayer")][JOAAT("m_iHealth")].get<int>(this);
        }

        inline int& team() {
            return netvars[JOAAT("DT_BaseEntity")][JOAAT("m_iTeamNum")].get<int>(this);
        }

        inline int& move_type() {
            return *reinterpret_cast<int*>(reinterpret_cast<uint32_t>(this) + 0x1A4);
        }

        inline vec3& origin() {
            return netvars[JOAAT("DT_BaseEntity")][JOAAT("m_vecOrigin")].get<vec3>(this);
        }

        inline vec3& abs_velocity() {
            return *reinterpret_cast<vec3*>(reinterpret_cast<uint32_t>(this) + 0x15C);
        }

        inline handle& active_weapon() {
            return netvars[JOAAT("DT_BaseCombatCharacter")][JOAAT("m_hActiveWeapon")].get<handle>(this);
        }

        inline int& ammo(int slot) {
            return netvars[JOAAT("DT_BasePlayer")][JOAAT("localdata")][JOAAT("m_iAmmo")].get<int>(this, slot);
        }

        inline int& clip_1() {
            return netvars[JOAAT("DT_BaseCombatWeapon")][JOAAT("LocalWeaponData")][JOAAT("m_iClip1")].get<int>(this);
        }

        inline int& primary_ammo_type() {
            return netvars[JOAAT("DT_BaseCombatWeapon")][JOAAT("LocalWeaponData")][JOAAT("m_iPrimaryAmmoType")].get<int>(this);
        }

        inline int& secondary_ammo_type() {
            return netvars[JOAAT("DT_BaseCombatWeapon")][JOAAT("LocalWeaponData")][JOAAT("m_iSecondaryAmmoType")].get<int>(this);
        }

        inline int& player_class() {
            return netvars[JOAAT("DT_TFPlayer")][JOAAT("m_PlayerClass")][JOAAT("m_iClass")].get<int>(this);
        }

        inline uint32_t& player_cond() {
            return netvars[JOAAT("DT_TFPlayer")][JOAAT("m_Shared")][JOAAT("m_nPlayerCond")].get<uint32_t>(this);
        }

        inline uint32_t& player_cond_ex_3() {
            return netvars[JOAAT("DT_TFPlayer")][JOAAT("m_Shared")][JOAAT("m_nPlayerCondEx3")].get<uint32_t>(this);
        }

        inline uint32_t& player_cond_ex() {
            return netvars[JOAAT("DT_TFPlayer")][JOAAT("m_Shared")][JOAAT("m_nPlayerCondEx")].get<uint32_t>(this);
        }

        inline uint32_t& player_cond_ex_2() {
            return netvars[JOAAT("DT_TFPlayer")][JOAAT("m_Shared")][JOAAT("m_nPlayerCondEx2")].get<uint32_t>(this);
        }

        inline uint32_t& player_cond_ex_4() {
            return netvars[JOAAT("DT_TFPlayer")][JOAAT("m_Shared")][JOAAT("m_nPlayerCondEx4")].get<uint32_t>(this);
        }

        inline uint32_t& cond_bits() {
            return netvars[JOAAT("DT_TFPlayer")][JOAAT("m_Shared")][JOAAT("m_ConditionList")][JOAAT("_condition_bits")].get<uint32_t>(this);
        }

        inline float& sim_time() {
            return netvars[JOAAT("DT_BaseEntity")][JOAAT("m_flSimulationTime")].get<float>(this);
        }

    	inline int& hitbox_set() {
	        return netvars[JOAAT("DT_BaseAnimating")][JOAAT("m_nHitboxSet")].get<int>(this);
        }

        inline int& tickbase() {
            return netvars[JOAAT("DT_BasePlayer")][JOAAT("localdata")][JOAAT("m_nTickBase")].get<int>(this);
        }

        inline ang& view_angle() {
            return netvars[JOAAT("DT_BasePlayer")][JOAAT("pl")][JOAAT("deadflag")].add(4).get<ang>(this);
        }

        inline ang& rotation() {
            return netvars[JOAAT("DT_BaseEntity")][JOAAT("m_angRotation")].get<ang>(this);
        }

        inline float& next_primary_attack() {
            return netvars[JOAAT("DT_BaseCombatWeapon")][JOAAT("LocalActiveWeaponData")][JOAAT("m_flNextPrimaryAttack")].get<float>(this);
        }

        inline float& next_secondary_attack() {
            return netvars[JOAAT("DT_BaseCombatWeapon")][JOAAT("LocalActiveWeaponData")][JOAAT("m_flNextSecondaryAttack")].get<float>(this);
        }

        inline bool& force_taunt_cam() {
            return netvars[JOAAT("DT_TFPlayer")][JOAAT("m_nForceTauntCam")].get<bool>(this);
        }

        inline user_cmd** current_cmd() {
            return reinterpret_cast<user_cmd**>(reinterpret_cast<uint32_t>(this) + 0x107C);
        }

        inline player_anim_state* anim_state() {
            return *reinterpret_cast<player_anim_state**>(reinterpret_cast<uint32_t>(this) + 0x1BF8);
        }
    };
}

#endif //EV0LVE_TF_ENTITY_H
