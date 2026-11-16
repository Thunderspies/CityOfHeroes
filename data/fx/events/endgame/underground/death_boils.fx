#############################################################
## Slime_Body.fx
#############################################################

FxInfo
Flags DontSuppress
Lifespan 200

########################################################



## POWER FX #######################################################

Condition
	On	Cycle
	Time	2
	Chance	0.8
	Random	1

	Event
		EName	Arm
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-1.5 0.0 0.0
			StartJitter	1.5 0.5 0.5
		End
		ChildFX	:Child_Boil_Pop.fx
		Lifespan 	61
	End

	Event
		EName	Arm
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	1.5 0.0 0.0
			StartJitter	1.5 0.5 0.5
		End
		ChildFX	:Child_Boil_Pop.fx
		Lifespan 	61
	End


	Event
		EName	Arm
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PYRRotateJitter	180 180 180
			PositionOffset	0.0 0.5 1.35
			StartJitter	1.5 1.0 0.5
		End
		ChildFX	:Child_Boil_Pop.fx
		Lifespan 	61
	End

	Event
		EName	Arm
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PYRRotateJitter	180 180 180
			PositionOffset	0.0 1.0 1.35
			StartJitter	1.5 0.5 0.5
		End
		ChildFX	:Child_Boil_Pop_Small.fx
		Lifespan 	61
	End

	Event
		EName	Arm
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-1.5 0.0 0.0
			StartJitter	1.5 0.5 0.5
		End
		ChildFX	:Child_Boil_Pop_Small.fx
		Lifespan 	61
	End

	Event
		EName	Arm
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	1.5 0.0 0.0
			StartJitter	1.5 0.5 0.5
		End
		ChildFX	:Child_Boil_Pop_Small.fx
		Lifespan 	61
	End


End

################################################################

