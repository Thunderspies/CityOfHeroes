#############################################################
## ScorpionShield.fx
#############################################################

FxInfo

#################################################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hips
		part1	:LegMotionGlows2.part
		Sound regen5b 60 60 0.5
	End

	Event
		Type	Local
		At	Hips
		part1	:LegMotionGlows2.part
		PMagnet	UlegL
		Sound glow5_loop 60 60 0.12
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 430
	End

	Event
		Type	Local
		At	Hips
		part1	:LegMotionGlows2.part
		PMagnet	UlegR
	End

	Event
		Type	Local
		At	LlegL
		part1	:LegMotionGlows2.part
		PMagnet	ULEGL
	End

	Event
		Type	Local
		At	LlegR
		part1	:LegMotionGlows2.part
		PMagnet	ULEGR
	End

	Event
		Type	Local
		At	LLEGL
		part1	:LegMotionGlows2.part
		PMagnet	FootL
	End

	Event
		Type	Local
		At	LLEGR
		part1	:LegMotionGlows2.part
		PMagnet	FootR
	End

	Event
		Type	Local
		At	FootR
		part1	:LegMotionGlows2.part
	End

	Event
		Type	Local
		At	FootL
		part1	:LegMotionGlows2.part
	End

	Event
		Type	Local
		At	FootR
		part1	:ArmMotionGlows2.part
		PMagnet LlegR
	End

	Event
		Type	Local
		At	ULEGL
		part1	:ArmMotionGlows2.part
	End

	Event
		Type	Local
		At	LLEGL
		part1	:ArmMotionGlows2.part
		PMagnet UlegL
	End

	Event
		Type	Local
		At	FootL
		part1	:ArmMotionGlows2.part
		PMagnet LlegL
	End

	Event
		Type	Local
		At	ULEGR
		part1	:ArmMotionGlows2.part
	End

	Event
		Type	Local
		At	LLEGR
		part1	:ArmMotionGlows2.part
		PMagnet UlegL
	End

	Event
		Type	Local
		At	Head
		part1	:MainMotionGlows2.part
	End

	Event
		Type	Local
		At	Chest
		part1	:MainMotionGlows2.part
	End

	Event
		Type	Local
		At	Neck
		part1	:MainMotionGlows2.part
		PMagnet UarmL
	End

	Event
		Type	Local
		At	Neck
		part1	:MainMotionGlows2.part
		PMagnet UarmR
	End

	Event
		Type	Local
		At	Neck
		part1	:LegMotionGlows2.part
	End

	Event
		Type	Local
		At	UArmL
		part1	:ArmMotionGlows2.part
	End

	Event
		Type	Local
		At	LArmL
		part1	:ArmMotionGlows2.part
	End

	Event
		Type	Local
		At	HandL
		part1	:ArmMotionGlows2.part
	End

	Event
		Type	Local
		At	UArmR
		part1	:ArmMotionGlows2.part
	End

	Event
		Type	Local
		At	LArmR
		part1	:ArmMotionGlows2.part
	End

	Event
		Type	Local
		At	HandR
		part1	:ArmMotionGlows2.part
	End
End

#############################################################

Condition
	On 	cycle
	Time 	20
	Chance	0.2

	Event
		Type	Local
		At	LarmL
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part
		Lifespan 25
	End
End

Condition
	On 	cycle
	Time 	19
	Chance	0.2

	Event
		Type	Local
		At	LarmR
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part
		Lifespan 25
	End
End

Condition
	On 	cycle
	Time 	21
	Chance	0.2

	Event
		Type	Local
		At	UarmL
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part
		Lifespan 25
	End
End

Condition
	On 	cycle
	Time 	22
	Chance	0.2

	Event
		Type	Local
		At	UarmR
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part
		Lifespan 25
	End
End

Condition
	On 	cycle
	Time 	17
	Chance	0.2

	Event
		Type	Local
		At	LlegL
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part
		Lifespan 25
	End
End

Condition
	On 	cycle
	Time 	18
	Chance	0.2

	Event
		Type	Local
		At	LlegR
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part
		Lifespan 25
	End
End

#####################################################################################

Condition
	On	Time
	Time	15

	Event
		Ename	Ring
		Type	Local
		At	Hips
		Part	:Glow01.part
	End
End

Condition
	On	Time
	Time	28

	Event
		Ename	Ring
		Type	Local
		At	Hips
		Part	:ERing02.part
		Part	:Cloud02.part
		Part	:Shield01.part
	End

	Event
		Ename	Logo
		Type	Local
		At	Chest
		Part	:Logo01.part
	End

	Event
		Ename	Logo
		Type	Local
		At	waist
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		Part	:Logo02.part
	End
End

#############################################################

Condition
	On	Death

	Event
		Type	Local
		At	Hips
		Part	:ERing01.part
		Part	:Cloud01.part
		Lifespan 1
	End
End

#############################################################

End

