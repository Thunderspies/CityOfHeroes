#############################################################
## emotes_RecreationBooster.inc
#############################################################

## Soccer Emote ###########################################################

#Move Emote_Soccer_Pre
#	NextMove	Emote_Soccer_Cycle
#
#	Type		male
#	  Anim          male\Soccer_idle 1
#	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
#	TEnd
#
#	Type		fem
#	  Anim          fem\Soccer_idle 1
#	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
#	TEnd
#
#	Type		huge
#	  Anim          huge\Soccer_idle 1
#	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
#	TEnd
#
#MEnd
#
#Move Emote_Soccer_Cycle
#	Include sequencers\Members\Emotes_Idle.inc
#	Flags		CYCLE nosizescale
#	Interpolate	0
#	CycleMove	Emote_Soccer_Cycle
#	CycleMove	Emote_Soccer_foot_juggle_pre
#
#	Type		male
#	  Anim          male\Soccer_idle 1
#	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
#	TEnd
#
#	Type		fem
#	  Anim          fem\Soccer_idle 1
#	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
#	TEnd
#
#	Type		huge
#	  Anim          huge\Soccer_idle 1
#	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
#	TEnd
#
#MEnd

Move Emote_Soccer_foot_juggle_pre
	Requires	EMOTE SOCCER
	Priority        2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	Interpolate	0
	NextMove	Emote_Soccer_foot_juggle

	Type		male
	  Anim          male\Soccer_juggle 1 20
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_FootFlip.fx
	  PlayFX		NPCFX\SoccerBall_Knee.fx 5
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 1 20
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_FootFlip.fx
	  PlayFX		NPCFX\SoccerBall_Knee.fx 5
	TEnd

	Type		huge
	  Anim          huge\Soccer_juggle 1 20
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_FootFlip.fx
	  PlayFX		NPCFX\SoccerBall_Knee.fx 5
	TEnd
MEnd

Move Emote_Soccer_foot_juggle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Soccer_foot_juggle
	CycleMove	Emote_Soccer_foot_juggle
	CycleMove	Emote_Soccer_knee_juggle_pre
	CycleMove	Emote_Soccer_foot_juggle_post

	Type		male
	  Anim          male\Soccer_juggle 21 47
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Kick.fx 13
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 21 47
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Kick.fx 13
	TEnd

	Type		huge
	  Anim          huge\Soccer_juggle 21 47
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Kick.fx 13
	TEnd
MEnd

Move Emote_Soccer_foot_juggle_post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Soccer_foot_juggle_pre

	Type		male
	  Anim          male\Soccer_juggle 170 194
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Catch.fx 12
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 170 194
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Catch.fx 12
	TEnd

	Type		huge
	  Anim          huge\Soccer_juggle 170 194
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Catch.fx 12
	TEnd
MEnd

Move Emote_Soccer_knee_juggle_pre
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Soccer_knee_juggle

	Type		male
	  Anim          male\Soccer_juggle 48 79
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Kick.fx 13
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 48 79
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Kick.fx 13
	TEnd

	Type		huge
	  Anim          huge\Soccer_juggle 48 79
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Kick.fx 13
	TEnd
MEnd

Move Emote_Soccer_knee_juggle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Soccer_knee_juggle
	CycleMove	Emote_Soccer_knee_juggle
	CycleMove	Emote_Soccer_head_juggle_pre
	CycleMove	Emote_Soccer_knee_juggle_post

	Type		male
	  Anim          male\Soccer_juggle 80 111
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 16
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 80 111
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 16
	TEnd

	Type		huge
	  Anim          huge\Soccer_juggle 80 111
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 16
	TEnd
MEnd

Move Emote_Soccer_knee_juggle_post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Soccer_foot_juggle

	Type		male
	  Anim          male\Soccer_juggle 200 215
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 15
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 200 215
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 15
	TEnd

	Type		huge
	  Anim          huge\Soccer_juggle 200 215
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 15
	TEnd
MEnd

Move Emote_Soccer_head_juggle_pre
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Soccer_head_juggle

	Type		male
	  Anim          male\Soccer_juggle 112 147
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 18
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 112 147
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 18
	TEnd

	Type		huge
	  Anim          huge\Soccer_juggle 112 147
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 18
	TEnd
MEnd

Move Emote_Soccer_head_juggle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Soccer_head_juggle
	CycleMove	Emote_Soccer_head_juggle
	CycleMove	Emote_Soccer_head_juggle
	CycleMove	Emote_Soccer_head_juggle_post

	Type		male
	  Anim          male\Soccer_juggle 148 163
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Head.fx 1
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 148 163
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Head.fx 1
	TEnd

	Type		huge
	  Anim          huge\Soccer_juggle 148 163
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Head.fx 1
	TEnd
MEnd

Move Emote_Soccer_head_juggle_post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Soccer_foot_juggle

	Type		male
	  Anim          male\Soccer_juggle 220 243
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Head.fx 1
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 220 243
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Head.fx 1
	TEnd

	Type		huge
	  Anim          huge\Soccer_juggle 220 243
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Head.fx 1
	TEnd
MEnd

#############################################################
##  Emote Basketball
#############################################################

#Move Emote_Basketball_Talk_Pre
#
#	NextMove	Emote_Basketball_Talk_Cycle
#
#	Type		male
#	  Anim          male\Basketball_talk_cycle 1
#	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
#	TEnd
#
#	Type		fem
#	  Anim          fem\Basketball_talk_cycle 1
#	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
#	TEnd
#
#	Type		huge
#	  Anim          huge\Basketball_talk_cycle 1
#	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
#	TEnd
#MEnd
#
#Move Emote_Basketball_Talk_Cycle
#	Include sequencers\Members\Emotes_Idle.inc
#	Flags		CYCLE nosizescale
#	Interpolate	0
#	CycleMove	Emote_Basketball_Talk_CycleBreak_A
#	CycleMove	Emote_Basketball_Talk_CycleBreak_B
#	CycleMove	Emote_Basketball_dribble_pre
#	CycleMove	Emote_Basketball_around_waist_pre
#
#	Type		male
#	  Anim          male\Basketball_talk_cycle 1
#	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
#	TEnd
#
#	Type		fem
#	  Anim          fem\Basketball_talk_cycle 1
#	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
#	TEnd
#
#	Type		huge
#	  Anim          huge\Basketball_talk_cycle 1
#	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
#	TEnd
#MEnd

#Move Emote_Basketball_Talk_CycleBreak_A
#
#	Flags		nosizescale
#	Interpolate	0
#	NextMove	Emote_Basketball_Talk_CycleBreak_B
#
#	Type		male
#	  Anim          male\basketball_idle_break_A 1
#	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
#	TEnd
#
#	Type		fem
#	  Anim          fem\basketball_idle_break_A 1
#	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
#	TEnd
#
#	Type		huge
#	  Anim          huge\basketball_idle_break_A 1
#	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
#	TEnd
#MEnd
#
#Move Emote_Basketball_Talk_CycleBreak_B
#	Include sequencers\Members\Emotes_Idle.inc
#	Flags		CYCLE nosizescale
#	Interpolate	0
#	CycleMove	Emote_Basketball_Talk_CycleBreak_A
#	CycleMove	Emote_Basketball_around_waist_pre
#
#	Type		male
#	  Anim          male\basketball_idle_break_B 1
#	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
#	TEnd
#
#	Type		fem
#	  Anim          fem\basketball_idle_break_B 1
#	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
#	TEnd
#
#	Type		huge
#	  Anim          huge\basketball_idle_break_B 1
#	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
#	TEnd
#MEnd

Move Emote_Basketball_around_waist_pre
	Requires	EMOTE BASKETBALL
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	Interpolate	0
	NextMove	Emote_Basketball_around_waist

	Type		male
	  Anim          male\Basketball_around_waist 1 11
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Basketball_around_waist 1 11
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\Basketball_around_waist 1 11
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Basketball_around_waist
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	8
	CycleMove	Emote_Basketball_around_waist
	CycleMove	Emote_Basketball_dribble_pre

	Type		male
	  Anim          male\Basketball_around_waist 12 43
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_HandHit.fx 16
	TEnd

	Type		fem
	  Anim          fem\Basketball_around_waist 12 43
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_HandHit.fx 16
	TEnd

	Type		huge
	  Anim          huge\Basketball_around_waist 12 43
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_HandHit.fx 16
	TEnd
MEnd

Move Emote_Basketball_around_waist_post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Basketball_dribble_pre

	Type		male
	  Anim          male\Basketball_around_waist 44
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Basketball_around_waist 44
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\Basketball_around_waist 44
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Basketball_dribble_pre
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Basketball_dribble

	Type		male
	  Anim          male\Basketball_dribble_cycle 1 11
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 1 11
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	TEnd

	Type		huge
	  Anim          huge\Basketball_dribble_cycle 1 11
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	TEnd
MEnd

Move Emote_Basketball_dribble
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Basketball_dribble
	CycleMove	Emote_Basketball_crossover_right_pre

	Type		male
	  Anim          male\Basketball_dribble_cycle 12 27
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 8
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 12 27
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 8
	TEnd

	Type		huge
	  Anim          huge\Basketball_dribble_cycle 12 27
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 8
	TEnd
MEnd

Move Emote_Basketball_dribble_post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Basketball_dribble

	Type		male
	  Anim          male\Basketball_dribble_cycle 168 197
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 8
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 168 197
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 8
	TEnd

	Type		huge
	  Anim          huge\Basketball_dribble_cycle 168 197
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 8
	TEnd
MEnd

Move Emote_Basketball_crossover_right_pre
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Basketball_crossover_right

	Type		male
	  Anim          male\Basketball_dribble_cycle 28 60
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 28 60
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd

	Type		huge
	  Anim          huge\Basketball_dribble_cycle 28 60
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd
MEnd

Move Emote_Basketball_crossover_right
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Basketball_crossover_right
	CycleMove	Emote_Basketball_crossover_right_post
	CycleMove	Emote_Basketball_crossover_left_pre

	Type		male
	  Anim          male\Basketball_dribble_cycle 61 99
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 61 99
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd

	Type		huge
	  Anim          huge\Basketball_dribble_cycle 61 99
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd
MEnd

Move Emote_Basketball_crossover_right_post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Basketball_dribble

	Type		male
	  Anim          male\Basketball_dribble_cycle 200 236
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 200 236
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd

	Type		huge
	  Anim          huge\Basketball_dribble_cycle 200 236
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd
MEnd

Move Emote_Basketball_crossover_left_pre
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Basketball_crossover_left

	Type		male
	  Anim          male\Basketball_dribble_cycle 100 114
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 100 114
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	TEnd

	Type		huge
	  Anim          huge\Basketball_dribble_cycle 100 114
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	TEnd
MEnd

Move Emote_Basketball_crossover_left
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Basketball_crossover_left
	CycleMove	Emote_Basketball_crossover_left_post

	Type		male
	  Anim          male\Basketball_dribble_cycle 115 155
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 115 155
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd

	Type		huge
	  Anim          huge\Basketball_dribble_cycle 115 155
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd
MEnd

Move Emote_Basketball_crossover_left_post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Emote_Basketball_crossover_right

	Type		male
	  Anim          male\Basketball_dribble_cycle 239 254
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 239 254
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	TEnd

	Type		huge
	  Anim          huge\Basketball_dribble_cycle 239 254
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	TEnd
MEnd

##################################################
## Pool Cue Emote ###########################################################
##################################################

Move Emote_PoolCue_Talking_Cycle
	Requires	EMOTE STAND POOL HIGH
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	Interpolate	8
	NextMove	emote_PoolCue_TalkBreakA_Cycle

	Type		male
	  Anim          male\poolcue_ready 1
	  PlayFx        GENERIC\PoolCue.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\poolcue_ready 1
	  PlayFx        GENERIC\PoolCue_Fem.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\poolcue_ready 1
	  PlayFx        GENERIC\PoolCue_Fem.fx 0 CONSTANT
	TEnd
MEnd

Move emote_PoolCue_TalkBreakA_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	Interpolate	8
	NextMove	emote_PoolCue_Chalking_Cycle
	CycleMove	emote_PoolCue_Swinging_Around

	Type		male
	  Anim          male\poolcue_talk_breakA 1
	  PlayFx        GENERIC\PoolCue.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\poolcue_talk_breakA 1
	  PlayFx        GENERIC\PoolCue_Fem.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\poolcue_talk_breakA 1
	  PlayFx        GENERIC\PoolCue_Fem.fx 0 CONSTANT
	TEnd
MEnd

Move emote_PoolCue_TalkBreakB_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NoSizeScale
	Interpolate	8
	CycleMove	emote_PoolCue_TalkBreakB_Cycle
	CycleMove	Emote_PoolCue_Talking_Cycle
	CycleMove	emote_PoolCue_Swinging_Around

	Type		male
	  Anim          male\poolcue_talk_breakB 1
	  PlayFx        GENERIC\PoolCue.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\poolcue_talk_breakB 1
	  PlayFx        GENERIC\PoolCue_Fem.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\poolcue_talk_breakB 1
	  PlayFx        GENERIC\PoolCue_Fem.fx 0 CONSTANT
	TEnd
MEnd

Move emote_PoolCue_Chalking_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NoSizeScale
	Interpolate	8
	CycleMove	emote_PoolCue_TalkBreakB_Cycle
	CycleMove	emote_PoolCue_Swinging_Around

	Type		male
	  Anim          male\poolcue_chalking 1
	  PlayFx        GENERIC\PoolCue.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\poolcue_chalking 1
	  PlayFx        GENERIC\PoolCue_Fem.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\poolcue_chalking 1
	  PlayFx        GENERIC\PoolCue_Fem.fx 0 CONSTANT
	TEnd
MEnd

Move emote_PoolCue_Swinging_Around
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NoSizeScale
	Interpolate	8
	CycleMove	Emote_PoolCue_Talking_Cycle
	CycleMove	emote_PoolCue_TalkBreakB_Cycle
	CycleMove	emote_PoolCue_TalkBreakA_Cycle


	Type		male
	  Anim          male\poolcue_goofingaround 1
	  PlayFx        GENERIC\PoolCue.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\poolcue_goofingaround 1
	  PlayFx        GENERIC\PoolCue_Fem.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\poolcue_goofingaround 1
	  PlayFx        GENERIC\PoolCue_Fem.fx 0 CONSTANT
	TEnd
MEnd

########################
## Open Holiday Gift  ##
########################

Move Emote_Open_Holiday_Gift
	Requires	EMOTE STAND GIFT LOW
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Flags		NoSizeScale
	NextMove        Ready

	Type		male
	  Anim		male\open_holiday_gift 1
	  PlayFx	Events\Winter\Open_Gift1.fx 1 Constant
	TEnd

	Type		fem
	  Anim          fem\open_holiday_gift 1
	  PlayFx        Events\Winter\Open_Gift1.fx 1 Constant
	TEnd

	Type		huge
	  Anim		huge\open_holiday_gift 1
	  PlayFx        Events\Winter\Open_Gift1.fx 1 Constant
	TEnd
MEnd
######################################################