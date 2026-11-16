#############################################################
## Sparks_Body.fx
#############################################################

FxInfo
Flags InheritAlpha InheritGeoScale

ClampMaxScale 3.5 3.5 3.5


#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Hairnode
		Type	Local
		At	hair
		Geom	HairToHeadAdjustment
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hairnode
		AltPiv	1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:InfestedConfuseDots01.part
		part	:InfestedConfuseDots02.part
		part	:InfestedConfuseBubbles01.part
		part	:InfestedConfuseStars01.part
	End
End



Condition
	On	Cycle
	Time	5
	Chance	0.85

	Event
		EName	Arm
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.1 0.1
			StartJitter	0.5 0.5 0.3
		End
		ChildFX	:InfestedConfuse_Chest_CHILD.fx
		Lifespan 	50
	End

End

Condition
	On	Cycle
	Time	4
	Chance	0.45
	Random	1

	Event
		EName	Arm
		Type	Local
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-0.35 0.025 0.0
			StartJitter	0.35 0.1 0.15
		End
		ChildFX	:InfestedConfuse_Pop_CHILD.fx
		Lifespan 	50
	End

	Event
		EName	Arm
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-0.35 0.025 0.0
			StartJitter	0.35 0.1 0.15
		End
		ChildFX	:InfestedConfuse_Pop_CHILD.fx
		Lifespan 	50
	End

###############################################################

	Event
		EName	Arm
		Type	Local
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.35 0.025 0.0
			StartJitter	0.35 0.1 0.15
		End
		ChildFX	:InfestedConfuse_Pop_CHILD.fx
		Lifespan 	50
	End

	Event
		EName	Arm
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.35 0.025 0.0
			StartJitter	0.35 0.1 0.15
		End
		ChildFX	:InfestedConfuse_Pop_CHILD.fx
		Lifespan 	50
	End


	Event
		EName	Head
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			#PositionOffset	-0.35 0.025 0.0
			StartJitter	0.35 0.5 0.15
		End
		ChildFX	:InfestedConfuse_Pop_CHILD.fx
		Lifespan 	50
	End

#############################################################

End