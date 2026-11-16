#############################################################
## Malignant_Continuing.fx
#############################################################

FxInfo
Flags DontSuppress

#########################################################
####################### AUDIO ###########################



#########################################################


Condition
	On 	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	HamidonSeed_Malignant_Continuing_Loop 100 100 .8
	End

	Event
		Type	Local
		At	Root
		SoundNoRepeat 1
		Sound 	HamidonSeed_Malignant_ContinuingChild_01 100 100 .8
		Sound 	HamidonSeed_Malignant_ContinuingChild_02 100 100 .8
		Sound 	HamidonSeed_Malignant_ContinuingChild_03 100 100 .8
		lifespan 1
	End

	Event
		Type	PositOnly
		At	Chest
		Bhvr 	Events\EndGame\KeyesIsland\ForceBubble_Disintegration.bhvr
		BhvrOverride
			Alpha		255
			StartColor	100 255 10
			PyrRotateJitter	-180.0 -180.0 0.0
			FadeInLength	20
			FadeOutLength	20
			Spin 		10 10 0
		End
		Part	:Malignant_Globs.part
		Part	:Malignant_Border.part
	End

	Event
		Type	Local
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Malignant_Tell_Alpha_ground.part
	end
End

#######################

Condition
	On 	Time
	Time 	5

	Event
		EName   RadiusFxScale
		Type	local
		At	Origin
		Bhvr	powers/firecontrol/Inferno.bhvr
		Part	:Malignant_Globs_Fast.part
		Lifespan 15
	End


	Event
		EName 	Prime1
		Type	StartPositOnly
		At	RadiusFxScale
		bhvr    behaviors/Braz/ExpandRingTo60.bhvr
		BhvrOverride
			ScaleRate		0.2 0.2 0.2
 			EndScale		9 9 9
		End
		Part1	:Malignant_Tick.part
		Part2	:Malignant_Dust.part
		Lifespan 60
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	RadiusFxScale

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		400
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName   RadiusFxScale
		Type	local
		At	Hips
		ChildFX	:Malignant_Continuing_CHILD.fx
	End
End

#########################################################

End