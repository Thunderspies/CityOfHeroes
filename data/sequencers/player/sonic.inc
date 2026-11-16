###########################################################
## SONIC.INC - Sequencers for Sonic powers (inc. Sounds)
###########################################################

## SONIC MOUTH - EYE BLAST ###########################################################

Move SonicMouth
	Requires	COMBAT HEADCAST SPEECH
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mouth_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\eye 1 49
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\eye 1 49
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\eye 1 49
	  PitchStart	1
	  PitchAngle	0.1
	TEnd
MEnd

#############################################################

Move A_SonicMouth
	Requires	FLY COMBAT HEADCAST SPEECH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Mouth_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_mouthcast 1 49
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\air_mouth 1 49
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\air_mouth 1 49
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

MEnd


## SONIC SKY SUMMON ###########################################################

Move S_SonicSummon
	Requires	COMBAT SKYSUMMON SPEECH
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        S_Summonpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\sky_summon 1 60
	TEnd

	Type		huge
	  Anim          huge\sky_summon 1 60
	TEnd

	Type		fem
	  Anim          fem\sky_summon 1 60
	TEnd
MEnd

#############################################################

Move A_S_SonicSummon
	Requires	FLY COMBAT SKYSUMMON SPEECH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        airS_Summonpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_skysummon 1 60
	TEnd

	Type		huge
	  Anim          huge\air_skysummon 1 60
	TEnd

	Type		fem
	  Anim          fem\air_skysummon 1 60
	TEnd
MEnd


## SONIC GROUND SUMMON ###########################################################

Move G_SonicSummon
	Requires	COMBAT GROUNDSUMMON SPEECH
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        G_Summonpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 1 104
	TEnd

	Type		huge
	  Anim          huge\ground_summon 1 104
	TEnd

	Type		fem
	  Anim          fem\ground_summon 1 104
	TEnd
MEnd

#############################################################

Move A_G_SonicSummon
	Requires	FLY COMBAT GROUNDSUMMON SPEECH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        airG_Summonpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_groundsummon 1 60
	TEnd

	Type		huge
	  Anim          huge\air_ground_summon 1 60
	TEnd

	Type		fem
	  Anim          fem\air_ground_summon 1 60
	TEnd
MEnd


## SONICSONIC YELL ###########################################################

Move SonicSonicYell
	Requires	COMBAT YELL STRONG ATTACK SPEECH
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SonicYell_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\sonic_yell 1 40
	TEnd

	Type		huge
	  Anim          huge\sonic_yell 1 40
	TEnd

	Type		fem
	  Anim          fem\sonic_yell 1 40
	TEnd
MEnd

#############################################################

Move A_SonicSonicYell
	Requires	FLY COMBAT YELL STRONG ATTACK SPEECH
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SonicYell_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_sonic_yell 1 40
	TEnd

	Type		huge
	  Anim          huge\air_sonic_yell 1 40
	TEnd
MEnd


## SONIC SING CAST ###########################################################

Move SonicSingCast
	Requires	COMBAT YELL YES CAST SPEECH
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SingCast_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\sing_cast 1 67
	TEnd

	Type		huge
	  Anim          huge\sing_cast 1 67
	TEnd
MEnd

#############################################################

Move A_SonicSingCast
	Requires	FLY COMBAT YELL YES CAST SPEECH
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SingCast_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_sing_cast 1 67
	TEnd
MEnd


## SONIC QUICK SPIT ###########################################################

Move SonicSpit
	Requires	COMBAT BLOW CAST QUICK SPEECH
	Priority        5
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SonicSpit_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\quickspit 1 31
	TEnd
MEnd

Move SonicSpit_Post
	Requires	COMBAT
	Priority        1
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <CombatAttack> <AttackIRQ> <BlockIRQ>
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\quickspit 32
	TEnd
MEnd

#############################################################

Move A_SonicSpit
	Requires	FLY COMBAT BLOW CAST QUICK SPEECH
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SonicSpit_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_quickspit 1 31
	TEnd
MEnd

Move A_SonicSpit_Post
	Requires	FLY COMBAT
	Priority        2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyCombat> <AttackIRQ> <BlockIRQ>
	NextMove	Lazy_C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_quickspit 32
	TEnd
MEnd


## SONIC SCREAM ###########################################################

Move SonicScream
	Requires	COMBAT SCREAM SPEECH
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Scream_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\scream 10 58
	    PlayFX	feet\MaleTaunt2.fx
	TEnd

	Type		huge
	  Anim          huge\scream 10 58
	  PlayFX	feet\MaleTaunt7.fx
	TEnd

	Type		fem
	  Anim          fem\scream 10 58
	  PlayFX	feet\femTaunt2.fx
	TEnd
MEnd

#############################################################

Move A_SonicScream
	Requires	FLY COMBAT SCREAM SPEECH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Scream_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_scream 10 58
	  PlayFX	feet\MaleTaunt2.fx
	TEnd

	Type		huge
	  Anim          huge\air_scream 10 58
	  PlayFX	feet\MaleTaunt7.fx
	TEnd

	Type		fem
	  Anim          fem\air_scream 10 58
	  PlayFX	feet\FemTaunt2.fx
	TEnd
MEnd


## SONIC BLOW CAST ###########################################################

Move SonicBlow
	Requires	COMBAT BLOW SPEECH
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Blowpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\blow_cast 1 79
	  PitchStart	12
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\blow_cast 1 79
	  PitchStart	12
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\blow_cast 1 79
	  PitchStart	12
	  PitchAngle	0.2
	TEnd
MEnd

#############################################################

Move A_SonicBlow
	Requires	FLY COMBAT BLOW SPEECH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Blowpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_blow_cast 1 79
	  PitchStart	12
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\air_blow_cast 1 79
	  PitchStart	12
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\air_blow_cast 1 79
	  PitchStart	12
	  PitchAngle	0.2
	TEnd
MEnd


## SONIC ENERGY TORRENT ###########################################################

Move SonicEnergy_Torrent
	Requires	COMBAT ENERGYTORRENT SPEECH
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        E_Torrent_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\energy_torrent 1 64
	TEnd

	Type		huge
	  Anim          huge\energy_torrent 1 64
	TEnd

	Type		fem
	  Anim          fem\energy_torrent 1 64
	TEnd
MEnd

#############################################################

Move A_SonicEnergy_Torrent
	Requires	FLY COMBAT ENERGYTORRENT SPEECH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_E_Torrent_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_energy_torrent 1 64
	TEnd

	Type		huge
	  Anim          huge\air_energy_torrent 1 64
	TEnd

	Type		fem
	  Anim          fem\air_energy_torrent 1 64
	TEnd
MEnd


## SONIC BOOSTUP ###########################################################

Move SonicBoostup
	Requires	BOOSTUP SPEECH
	Priority        2
	Interrupts	<ReactIRQ> <HitIRQ> <AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Boostuppost
	Flags		NoSizeScale

	Type		male
	  Anim          male\absorb 1 10
	TEnd

	Type		huge
	  Anim          huge\boostup 1 10
	TEnd

	Type		fem
	  Anim          fem\boostup 1 10
	TEnd

MEnd

#############################################################

Move A_SonicBoostup
	Requires	FLY BOOSTUP SPEECH
	Priority        3
	Interrupts	<ReactIRQ> <HitIRQ> <AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> <FlyReady>
	NextMove        A_Boostuppost
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_boostup 1 10
	TEnd

	Type		huge
	  Anim          huge\air_boostup 1 10
	TEnd

	Type		fem
	  Anim          fem\air_boostup 1 10
	TEnd
MEnd


## SONIC TELEKINESIS QUICK ###########################################################

Move Sonic_Telequick
	Requires	COMBAT ATTACK TELEKINESIS SPEECH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sonic_Telequickpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\telekinesis 2 96
	  PitchStart	9
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\telekinesis 2 96
	  PitchStart	9
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\telekinesis 2 96
	  PitchStart	9
	  PitchAngle	0.5
	TEnd
MEnd

Move Sonic_Telequickpost
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ>
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\telekinesis 97
	TEnd

	Type		huge
	  Anim          huge\telekinesis 97
	TEnd

	Type		fem
	  Anim          fem\telekinesis 97
	TEnd
MEnd

#############################################################

Move A_SonicTelequick
	Requires	FLY COMBAT ATTACK TELEKINESIS SPEECH
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SonicTelequick2
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_telekinesis 2 51
	  PitchStart	9
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_telekinesis 2 51
	  PitchStart	9
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_telekinesis 2 51
	  PitchStart	9
	  PitchAngle	0.5
	TEnd
MEnd

Move A_SonicTelequick2
	Requires	FLY COMBAT
	Priority        2
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SonicTelequickpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_telekinesis 53 92
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_telekinesis 53 92
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_telekinesis 53 92
	  PitchAngle	0.5
	TEnd
MEnd

Move A_SonicTelequickpost
	Requires	FLY COMBAT
	Priority        2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyCombatAttack> <AttackIRQ> <BlockIRQ>
	NextMove        C_flyready
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_telekinesis 93
	  PitchEnd	96
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_telekinesis 93
	  PitchEnd	96
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_telekinesis 93
	  PitchEnd	96
	  PitchAngle	0.5
	TEnd
MEnd


## AIR SONIC HYPNOTIZE ###########################################################

Move A_SonicHypnotize
	Requires	FLY COMBAT HYPNOTIZE SPEECH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SonicHypnotize_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale           1.50

	Type		male
	  Anim          male\air_hypnotize 1 90
	TEnd

	Type		huge
	  Anim          huge\air_hypnotize 1 90
	TEnd

	Type		fem
	  Anim          fem\air_hypnotize 1 90
	TEnd
MEnd

Move A_SonicHypnotize_Post
	Requires	FLY COMBAT
	Priority        2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyCombatAttack> <BlockIRQ> <AttackIRQ>
	NextMove        A_Hypnotize2
	Scale           1.50
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_hypnotize 91 133
	  PitchEnd	167
	TEnd

	Type		huge
	  Anim          huge\air_hypnotize 91 133
	  PitchEnd	167
	TEnd

	Type		fem
	  Anim          fem\air_hypnotize 91 133
	  PitchEnd	167
	TEnd
MEnd