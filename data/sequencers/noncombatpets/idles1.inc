#############################################################
## SPROCKET PET IDLE BREAKS
#############################################################

Move IdleBridge
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	Ready2
	CycleMove	Random1
	CycleMove	Random2

	Type		male
	  Anim          male\Skel_ready2 1 1
	TEnd
MEnd

#############################################################

Move Random1
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	IdleBreakA_1
	CycleMove	IdleBreakB_1
	CycleMove	IdleBreakC_1
	CycleMove	IdleBreakD_1

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd
MEnd

Move Random2
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	IdleCycleA_1
	CycleMove	IdleCycleB_1
	CycleMove	IdleCycleC_1
	CycleMove	IdleCycleD_1

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd
MEnd

#############################################################
## IDLE BREAKS
#############################################################

Move IdleBreakA_1
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	IdleBridge
	CycleMove	IdleBreakA_1
	CycleMove	IdleBreakA_2
	Scale		0.5

	Type		male
	  Anim          male\PeerIn 1
	TEnd
MEnd

Move IdleBreakA_2
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	IdleBridge
	CycleMove	IdleBreakA_1
	CycleMove	IdleBreakA_2
	Scale		0.5

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

#############################################################

Move IdleBreakB_1
	Include sequencers\Members\Readies.inc
	NextMove	Ready_2Hips

	Type		male
	  Anim          male\emote_Chicken 1
	TEnd
MEnd

#############################################################

Move IdleBreakC_1
	Include sequencers\Members\Readies.inc
	NextMove	IdleBridge

	Type		male
	  Anim          male\emote_knuckle_crack 1 80
	TEnd
MEnd

#############################################################

Move IdleBreakD_1
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	IdleBridge
	CycleMove	IdleBreakA_1
	CycleMove	IdleBreakA_2
	Scale		0.5

	Type		male
	  Anim          male\PeerIn 1
	TEnd
MEnd

Move IdleBreakD_2
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	IdleBridge
	CycleMove	IdleBreakA_1
	CycleMove	IdleBreakA_2
	Scale		0.5

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

#############################################################
## IDLE CYCLES
#############################################################

Move IdleCycleA_1
	Include sequencers\Members\Readies.inc
	NextMove	IdleCycleA_2

	Type		male
	  Anim          male\sit_ground 1 102
	TEnd
MEnd

Move IdleCycleA_2
	Include sequencers\Members\Readies.inc
	Flags		Cycle

	Type		male
	  Anim          male\sit_ground 102
	TEnd
MEnd

#############################################################

Move IdleCycleB_1
	Include sequencers\Members\Readies.inc
	NextMove	IdleCycleB_2
	Flags		NoSizeScale
	Scale		1.25

	Type		male
	  Anim          male\Magic_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Electric_Male.fx 0 CONSTANT
	TEnd
MEnd

Move IdleCycleB_2
	Include sequencers\Members\Readies.inc
	Flags		Cycle NoSizeScale
	Scale		1.25

	Type		male
	  Anim		male\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Electric_Male.fx 0 CONSTANT
	TEnd
MEnd

##############################################################
#
#Move IdleCycleC_1
#	Include sequencers\Members\Readies.inc
#	NextMove	IdleCycleC_2
#	scale		0.9
#
#	Type		male
#	  Anim          male\emote_sleep 0 95
#	TEnd
#MEnd
#
#Move IdleCycleC_2
#	Include sequencers\Members\Emotes_Idle.inc
#	Flags		CYCLE
#
#	Type		male
#	  Anim          male\emote_Sleep 95 191
#	  PlayFX	V_COV\Items\Emote_SleepZ.fx 0 CONSTANT
#	TEnd
#MEnd

#############################################################

Move IdleCycleC_1
	Include sequencers\Members\Readies.inc
	NextMove	IdleCycleC_2

	Type		male
	  Anim          male\emote_waiting_cycle 1
	TEnd
MEnd

Move IdleCycleC_2
	Include sequencers\Members\Readies.inc
	Flags		FIXARMREG CYCLE
	CycleMove	IdleCycleC_2
	CycleMove	IdleCycleC_2
	CycleMove	IdleCycleC_3

	Type		male
	  Anim          male\emote_waiting_cycle 1
	TEnd
MEnd

Move IdleCycleC_3
	Include sequencers\Members\Readies.inc
	Flags		FIXARMREG
	NextMove	emote_Waiting_post

	Type		male
	  Anim          male\emote_waiting_break1 1
	TEnd
MEnd

#############################################################

Move IdleCycleD_1
	Include sequencers\Members\Readies.inc
	Flags		CYCLE

	Type		male
	  Anim          male\dance_washer 1
	TEnd
MEnd