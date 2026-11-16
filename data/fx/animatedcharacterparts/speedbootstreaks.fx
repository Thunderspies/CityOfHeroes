#############################################################
## FX System Name
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hair
		bhvr	Behaviors/GenericParticleFade.bhvr
		part2	:MotionGlows2.part
		#POther	Head
	End

	Event
		Type	Local
		At	Neck
		bhvr	Behaviors/GenericParticleFade.bhvr
		part2	:MotionGlows.part
		#POther	Chest
	End

	Event
		Type	Local
		At	UArmL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:MotionGlows2.part
		POther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:MotionGlows2.part
		POther	LArmR
	End

	Event
		Type	Local
		At	LArmL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:MotionGlows2.part
		POther	HandL
	End

	Event
		Type	Local
		At	LArmR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:MotionGlows2.part
		POther	HandR
	End

	Event
		Type	Local
		At	HandL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:MotionGlows2.part
	End

	Event
		Type	Local
		At	HandR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:MotionGlows2.part
	End
				
	Event
		Type	Local
		At	Waist
		bhvr	Behaviors/GenericParticleFade.bhvr
		part2	:MotionGlows.part
	End

	Event
		Type	Local
		At	Hips
		bhvr	Behaviors/GenericParticleFade.bhvr
		part2	:MotionGlows.part
	End

	Event
		Type	Local
		At	ULEGL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:MotionGlows2.part
		POther	LlegL
	End

	Event
		Type	Local
		At	ULEGR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:MotionGlows2.part
		POther	LlegR
	End

	Event
		Type	Local
		At	LLegL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:MotionGlows3.part
		POther	FootL
	End

	Event
		Type	Local
		At	LLegR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:MotionGlows3.part
		POther	FootR
	End

	Event
		Type	Local
		At	FootL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:MotionGlows3.part
	End
	
	Event
		Type	Local
		At	FootR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:MotionGlows3.part
	End
End

#############################################################

End		