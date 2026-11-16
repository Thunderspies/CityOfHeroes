#############################################################
## Particle System
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	15

	Event
		EName	2
		Type	Local
		At	Chest
		Sound forcefield5_loop 60 60 .66
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 180
	End
End

Condition
	On	Time
	Time	15

	Event
		EName	9
		Type	Local
		At	Head
		part2	:StrengthHeadGlows.part
		Sound rage 100 100 1.0
	End

	Event
		EName	9
		Type	Local
		At	Waist
		part2	:Rage_BigFlare1.part
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:Rage_ArmMotionGlows.part
		PMagnet LArmL
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:Rage_ArmMotionGlows.part
		PMagnet WepL
	End

#	Event
#		EName	12
#		Type	Local
#		At	WepL
#		part1	:Rage_ArmGlows.part
#	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:Rage_ArmMotionGlows.part
		PMagnet LArmR
	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:Rage_ArmMotionGlows.part
		PMagnet WepR
	End

#	Event
#		EName	15
#		Type	Local
#		At	WepR
#		part1	:Rage_ArmGlows.part
#	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:Rage_MotionGlows.part
		PMagnet LlegR
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:Rage_MotionGlows.part
		PMagnet FootR
	End
	
#	Event
#		EName	2
#		Type	Local
#		At	FootR
#		part1	:Rage_BodyGlows.part
#	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:Rage_MotionGlows.part
		PMagnet LLEGL
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:Rage_MotionGlows.part
		PMagnet FootL
	End

#	Event
#		EName	5		
#		Type	Local
#		At	FootL
#		part1	:Rage_BodyGlows.part
#	End
End    

#############################################################

End			