##############################################################
### Test Sequencers
##############################################################
#
#
##########################################################################

#Move Staff_postCycle
#	Include sequencers\Members\Emotes_Idle.inc
#	Flags		Cycle
#
#	Type		male
#	  Anim          male\2hlarge_ready 1 31
#	TEnd
#
#MEnd
#
##############################################################
#
#Move Mercurial_blow_A_pre
#	Requires	TEST1
#	Priority        1
#	Interrupts	<ReactIRQ>
#	Member		<DeathIRQ>
#	NextMove	Mercurial_blow_A
#	Flags		NoSizeScale
#	Interpolate	0
#
#	Type		male
#	  Anim		male\staff_mercurial_blow_A 1 7
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Mercurial_blow_A
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Mercurial_blow_A_post
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\staff_mercurial_blow_A 7 30
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Mercurial_blow_A_post
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Staff_postCycle
#
#	Type		male
#	  Anim          male\staff_mercurial_blow_A 31 67
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
###############################################################
#
#Move Mercurial_blow_B_pre
#	Requires	TEST1
#	Priority        1
#	Interrupts	<ReactIRQ>
#	Member		<DeathIRQ>
#	NextMove	Mercurial_blow_B
#	Flags		NoSizeScale
#	Interpolate	0
#
#	Type		male
#	  Anim		male\staff_mercurial_blow_B 1 7
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Mercurial_blow_B
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Mercurial_blow_B_post
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\staff_mercurial_blow_B 7 30
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Mercurial_blow_B_post
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Staff_postCycle
#
#	Type		male
#	  Anim          male\staff_mercurial_blow_B 31 67
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
###############################################################
#
#Move Mercurial_blow_C_pre
#	Requires	TEST1
#	Priority        1
#	Interrupts	<ReactIRQ>
#	Member		<DeathIRQ>
#	NextMove	Mercurial_blow_C
#	Flags		NoSizeScale
#	Interpolate	0
#
#	Type		male
#	  Anim		male\staff_mercurial_blow_C 1 7
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Mercurial_blow_C
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Mercurial_blow_C_post
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\staff_mercurial_blow_C 7 30
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Mercurial_blow_C_post
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Staff_postCycle
#
#	Type		male
#	  Anim          male\staff_mercurial_blow_C 31 67
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
###############################################################
#
#Move Mercurial_blow_D_pre
#	Requires	TEST1
#	Priority        1
#	Interrupts	<ReactIRQ>
#	Member		<DeathIRQ>
#	NextMove	Mercurial_blow_D
#	Flags		NoSizeScale
#	Interpolate	0
#
#	Type		male
#	  Anim		male\staff_mercurial_blow_D 1 7
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Mercurial_blow_D
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Mercurial_blow_D_post
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\staff_mercurial_blow_D 7 30
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Mercurial_blow_D_post
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Staff_postCycle
#
#	Type		male
#	  Anim          male\staff_mercurial_blow_D 31 67
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
###############################################################
#
#Move Precise_strike_pre
#	Requires	TEST2
#	Priority        1
#	Interrupts	<ReactIRQ>
#	Member		<DeathIRQ>
#	NextMove	Precise_strike
#	Flags		NoSizeScale
#	Interpolate	0
#
#	Type		male
#	  Anim		male\staff_precise_strike 1 10
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Precise_strike
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Precise_strike_post
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\staff_precise_strike 11 34
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Precise_strike_post
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Staff_postCycle
#
#	Type		male
#	  Anim          male\staff_precise_strike 35 45
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
###############################################################
#
#Move Guard_spin_pre
#	Requires	TEST3
#	Priority        1
#	Interrupts	<ReactIRQ>
#	Member		<DeathIRQ>
#	NextMove	Guard_spin
#	Flags		NoSizeScale
#	Interpolate	0
#
#	Type		male
#	  Anim		male\staff_guard_spin 1 1
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Guard_spin
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Guard_spin_post
#	Flags		NoSizeScale
#	Interpolate	0
#
#	Type		male
#	  Anim          male\staff_Guard_spin 2 50
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Guard_spin_post
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Staff_postCycle
#	Interpolate	0
#
#	Type		male
#	  Anim          male\staff_Guard_spin 51 70
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
###############################################################
#
#Move Innocuous_strike_pre
#	Requires	TEST4
#	Priority        1
#	Interrupts	<ReactIRQ>
#	Member		<DeathIRQ>
#	NextMove	Innocuous_strike
##	Flags		NoSizeScale
#	Interpolate	0
#
#	Type		male
#	  Anim		male\staff_innocuous_strike 1 1
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Innocuous_strike
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Innocuous_strike_post
##	Flags		NoSizeScale
#
#
#	Type		male
#	  Anim          male\staff_innocuous_strike 2 64
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Innocuous_strike_post
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Staff_postCycle
#
#
#	Type		male
#	  Anim          male\staff_innocuous_strike 65 102
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
###############################################################
#
#Move Staff_taunt_pre
#	Requires	TEST5
#	Priority        1
#	Interrupts	<ReactIRQ>
#	Member		<DeathIRQ>
#	NextMove	Staff_taunt
##	Flags		NoSizeScale
#
#
#	Type		male
#	  Anim		male\staff_taunt 1 1
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Staff_taunt
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Staff_taunt_post
##	Flags		NoSizeScale
#
#
#	Type		male
#	  Anim          male\staff_taunt 2 75
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Staff_taunt_post
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Staff_postCycle
#
#
#	Type		male
#	  Anim          male\staff_taunt 76 108
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
###############################################################
#
#Move Staff_eye_of_storm_pre
#	Requires	TEST6
#	Priority        1
#	Interrupts	<ReactIRQ>
#	Member		<DeathIRQ>
#	NextMove	Staff_eye_of_storm
#	Interpolate	0
##	Flags		NoSizeScale
#
#
#	Type		male
#	  Anim		male\Staff_eye_of_storm 1 1
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Staff_eye_of_storm
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Staff_eye_of_storm_post
#	Interpolate	0
##	Flags		NoSizeScale
#
#
#	Type		male
#	  Anim          male\Staff_eye_of_storm 2 80
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Staff_eye_of_storm_post
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Staff_postCycle
#	Interpolate	0
#
#
#	Type		male
#	  Anim          male\Staff_eye_of_storm 81 90
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
###############################################################
#
#Move Staff_serpent_reach_pre
#	Requires	TEST7
#	Priority        1
#	Interrupts	<ReactIRQ>
#	Member		<DeathIRQ>
#	NextMove	Staff_serpent_reach
#	Interpolate	0
##	Flags		NoSizeScale
#
#
#	Type		male
#	  Anim		male\Staff_serpent_reach 1 1
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Staff_serpent_reach
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Staff_serpent_reach_post
#	Interpolate	0
##	Flags		NoSizeScale
#
#
#	Type		male
#	  Anim          male\Staff_serpent_reach 2 53
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Staff_serpent_reach_post
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Staff_postCycle
#	Interpolate	0
#
#
#	Type		male
#	  Anim          male\Staff_serpent_reach 54 65
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
###############################################################
#
#Move Staff_sky_splitter_pre
#	Requires	TEST8
#	Priority        1
#	Interrupts	<ReactIRQ>
#	Member		<DeathIRQ>
#	NextMove	Staff_sky_splitter
#	Interpolate	0
##	Flags		NoSizeScale
#
#
#	Type		male
#	  Anim		male\Staff_sky_splitter 1 1
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Staff_sky_splitter
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Staff_sky_splitter_post
#	Interpolate	0
##	Flags		NoSizeScale
#
#
#	Type		male
#	  Anim          male\Staff_sky_splitter 2 85
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Staff_sky_splitter_post
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Staff_postCycle
#	Interpolate	0
#
#
#	Type		male
#	  Anim          male\Staff_sky_splitter 86 128
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
###############################################################
#
#Move Staff_form_of_body_pre
#	Requires	TEST9
#	Priority        1
#	Interrupts	<ReactIRQ>
#	Member		<DeathIRQ>
#	NextMove	Staff_form_of_body
#	Interpolate	0
##	Flags		NoSizeScale
#
#
#	Type		male
#	  Anim		male\Staff_mastery 1 1
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Staff_form_of_body
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Staff_form_of_body_post
#	Interpolate	0
##	Flags		NoSizeScale
#
#
#	Type		male
#	  Anim          male\Staff_mastery 2 40
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd
#
#Move Staff_form_of_body_post
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Staff_postCycle
#	Interpolate	0
#
#
#	Type		male
#	  Anim          male\Staff_mastery 41 55
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd

##############################################################

###############################################################
#Move TestEmote4
#	Requires	TEST4
#	Priority        1
#	Interrupts	<ReactIRQ> <HitIRQ> Stun Stun_cycle
#	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
#	Flags		NoSizeScale
#	NextMove	TestEmote4_fast
#
#	Type		male
#	  Anim		male\titanweapon_crushingblow_fast 1 10
#	TEnd
#
#	Type		huge
#	  Anim		huge\titanweapon_crushingblow_fast 1 10
#	TEnd
#
#	Type		fem
#	  Anim		fem\titanweapon_crushingblow_fast 1 10
#	TEnd
#
#MEnd
#
#Move TestEmote4_fast
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	TestEmote4_Post
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\titanweapon_crushingblow_fast 11 30
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_crushingblow_fast 11 30
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_crushingblow_fast 11 30
#	TEnd
#MEnd
#
#Move TestEmote4_Post
#	Include sequencers\Members\Emotes_Idle.inc
#
#	Type		male
#	  Anim          male\titanweapon_crushingblow_fast 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_crushingblow_fast 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_crushingblow_fast 31
#	TEnd
#
#MEnd
###############################################################
#
#Move TestEmote5
#	Requires	TEST5
#	Priority        1
#	Interrupts	<ReactIRQ> <HitIRQ> Stun Stun_cycle
#	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
#	Flags		NoSizeScale
#	NextMove	TestEmote5_fast
#
#	Type		male
#	  Anim		male\titanweapon_defensivesweep_slow 1 25
#	TEnd
#
#	Type		huge
#	  Anim		huge\titanweapon_defensivesweep_slow 1 25
#	TEnd
#
#	Type		fem
#	  Anim		fem\titanweapon_defensivesweep_slow 1 25
#	TEnd
#
#MEnd
#
#Move TestEmote5_fast
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	TestEmote5_Post
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\titanweapon_defensivesweep_slow 26 73
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_defensivesweep_slow 26 73
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_defensivesweep_slow 26 73
#	TEnd
#
#MEnd
#
#Move TestEmote5_Post
#	Include sequencers\Members\Emotes_Idle.inc
#
#	Type		male
#	  Anim          male\titanweapon_defensivesweep_slow 73
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_defensivesweep_slow 73
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_defensivesweep_slow 73
#	TEnd
#
#MEnd
###############################################################
#
#Move TestEmote6
#	Requires	TEST6
#	Priority	1
#	Interrupts	<ReactIRQ> <HitIRQ> Stun Stun_cycle
#	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
#	Flags		NoSizeScale
#	NextMove	TestEmote6_fast
#
#	Type		male
#	  Anim		male\titanweapon_defensivesweep_fast 1 10
#	TEnd
#
#	Type		huge
#	  Anim		huge\titanweapon_defensivesweep_fast 1 10
#	TEnd
#
#	Type		fem
#	  Anim		fem\titanweapon_defensivesweep_fast 1 10
#	TEnd
#
#MEnd
#
#Move TestEmote6_fast
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	TestEmote6_Post
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\titanweapon_defensivesweep_fast 11 30
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_defensivesweep_fast 11 30
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_defensivesweep_fast 11 30
#	TEnd
#
#MEnd
#
#Move TestEmote6_Post
#	Include sequencers\Members\Emotes_Idle.inc
#
#	Type		male
#	  Anim          male\titanweapon_defensivesweep_fast 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_defensivesweep_fast 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_defensivesweep_fast 31
#	TEnd
#
#MEnd
###############################################################
#Move TestEmote7
#	Requires	TEST7
#	Priority	1
#	Interrupts	<ReactIRQ> <HitIRQ> Stun Stun_cycle
#	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
#	NextMove	TestEmote7_fast
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim		male\titanweapon_follow_thru_fast 1 10
#	TEnd
#
#	Type		huge
#	  Anim		huge\titanweapon_follow_thru_fast 1 10
#	TEnd
#
#	Type		fem
#	  Anim		fem\titanweapon_follow_thru_fast 1 10
#	TEnd
#MEnd
#
#Move TestEmote7_fast
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	TestEmote7_Post
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\titanweapon_follow_thru_fast 11 30
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_follow_thru_fast 11 30
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_follow_thru_fast 11 30
#	TEnd
#
#MEnd
#
#Move TestEmote7_Post
#	Include sequencers\Members\Emotes_Idle.inc
#
#	Type		male
#	  Anim          male\titanweapon_follow_thru_fast 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_follow_thru_fast 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_follow_thru_fast 31
#	TEnd
#
#MEnd
#
###############################################################
#Move TestEmote8
#	Requires	TEST8
#	Priority	1
#	Interrupts	<ReactIRQ> <HitIRQ> Stun Stun_cycle
#	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
#	NextMove	TestEmote8_fast
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim		male\titanweapon_whirling_strike_fast 1 10
#	TEnd
#
#	Type		huge
#	  Anim		huge\titanweapon_whirling_strike_fast 1 10
#	TEnd
#
#	Type		fem
#	  Anim		fem\titanweapon_whirling_strike_fast 1 10
#	TEnd
#MEnd
#
#Move TestEmote8_fast
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	TestEmote8_Post
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\titanweapon_whirling_strike_fast 11 30
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_whirling_strike_fast 11 30
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_whirling_strike_fast 11 30
#	TEnd
#
#MEnd
#
#Move TestEmote8_Post
#	Include sequencers\Members\Emotes_Idle.inc
#
#	Type		male
#	  Anim          male\titanweapon_whirling_strike_fast 30
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_whirling_strike_fast 30
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_whirling_strike_fast 30
#	TEnd
#
#MEnd
#
################################################################
#Move TestEmote9
#	Requires	TEST9
#	Priority	1
#	Interrupts	<ReactIRQ> <HitIRQ> Stun Stun_cycle
#	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
#	NextMove	TestEmote9_fast
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim		male\titanweapon_sweeping_strike_slow 1 25
#	TEnd
#
#	Type		huge
#	  Anim		huge\titanweapon_sweeping_strike_slow 1 25
#	TEnd
#
#	Type		fem
#	  Anim		fem\titanweapon_sweeping_strike_slow 1 25
#	TEnd
#MEnd
#
#Move TestEmote9_fast
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	TestEmote9_Post
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\titanweapon_sweeping_strike_slow 26 87
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_sweeping_strike_slow 26 87
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_sweeping_strike_slow 26 87
#	TEnd
#
#MEnd
#
#Move TestEmote9_Post
#	Include sequencers\Members\Emotes_Idle.inc
#
#	Type		male
#	  Anim          male\titanweapon_sweeping_strike_slow 87
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_sweeping_strike_slow 87
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_sweeping_strike_slow 87
#	TEnd
#
#MEnd

###############################################################
#Move TestEmote9
#	Requires	TEST9
#	Priority	1
#	Interrupts	<ReactIRQ> <HitIRQ> Stun Stun_cycle
#	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
#	NextMove	TestEmote9_fast
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim		male\titanweapon_sweeping_strike_fast 1 10
#	TEnd
#
#	Type		huge
#	  Anim		huge\titanweapon_sweeping_strike_fast 1 10
#	TEnd
#
#	Type		fem
#	  Anim		fem\titanweapon_sweeping_strike_fast 1 10
#	TEnd
#MEnd
#
#Move TestEmote9_fast
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	TestEmote9_Post
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\titanweapon_sweeping_strike_fast 11 30
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_sweeping_strike_fast 11 30
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_sweeping_strike_fast 11 30
#	TEnd
#
#MEnd
#
#Move TestEmote9_Post
#	Include sequencers\Members\Emotes_Idle.inc
#
#	Type		male
#	  Anim          male\titanweapon_sweeping_strike_fast 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_sweeping_strike_fast 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_sweeping_strike_fast 31
#	TEnd
#
#MEnd
#
################################################################
#Move TestEmote9
#	Requires	TEST9
#	Priority	1
#	Interrupts	<ReactIRQ> <HitIRQ> Stun Stun_cycle
#	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
#	NextMove	TestEmote9_fast
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim		male\titanweapon_wave_of_destruction_slow 1 25
#	TEnd
#
#	Type		huge
#	  Anim		huge\titanweapon_wave_of_destruction_slow 1 25
#	TEnd
#
#	Type		fem
#	  Anim		fem\titanweapon_wave_of_destruction_slow 1 25
#	TEnd
#MEnd
#
#Move TestEmote9_fast
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	TestEmote9_Post
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\titanweapon_wave_of_destruction_slow 26 120
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_wave_of_destruction_slow 26 120
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_wave_of_destruction_slow 26 120
#	TEnd
#
#
#MEnd
#
#Move TestEmote9_Post
#	Include sequencers\Members\Emotes_Idle.inc
#
#	Type		male
#	  Anim          male\titanweapon_wave_of_destruction_slow 120
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_wave_of_destruction_slow 120
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_wave_of_destruction_slow 120
#	TEnd
#
#MEnd
#
################################################################
#Move TestEmote9
#	Requires	TEST9
#	Priority	1
#	Interrupts	<ReactIRQ> <HitIRQ> Stun Stun_cycle
#	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
#	NextMove	TestEmote9_fast
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim		male\titanweapon_wave_of_destruction_fast 1 10
#	TEnd
#
#	Type		huge
#	  Anim		huge\titanweapon_wave_of_destruction_fast 1 10
#	TEnd
#
#	Type		fem
#	  Anim		fem\titanweapon_wave_of_destruction_fast 1 10
#	TEnd
#MEnd
#
#Move TestEmote9_fast
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	TestEmote9_Post
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\titanweapon_wave_of_destruction_fast 11 30
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_wave_of_destruction_fast 11 30
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_wave_of_destruction_fast 11 30
#	TEnd
#
#
#MEnd
#
#Move TestEmote9_Post
#	Include sequencers\Members\Emotes_Idle.inc
#
#	Type		male
#	  Anim          male\titanweapon_wave_of_destruction_fast 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\titanweapon_wave_of_destruction_fast 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\titanweapon_wave_of_destruction_fast 31
#	TEnd
#
#MEnd

################################################################