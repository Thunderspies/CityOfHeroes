#########################################################
##	template

FxInfo
Lifespan 100



#########################################################
#####################  CLOUD 1  #########################
#########################################################

Condition
	On 	Time
	Time	0

	Event
		Type	Local
		At 	Origin
		Sound SS04 120 120 .9
	End
End




Condition

	On 	Time
	Time	0

	Event
		ENAME	OverHead
		Type	Local
		At	Root
		Geom	RoottoChestAdjustment
		#Lookat	Target

	End

End

Condition

	On 	Time
	Time	30

	Event
		ENAME	Mist
		Type	Start
		At	OverHead
		AltPiv	1
		part	CustomizeablePowers\StormSummoning\SSGaleMist01.part
		part	CustomizeablePowers\StormSummoning\SSGaleStreaks01.part
		part	CustomizeablePowers\StormSummoning\SSGaleChunks01.part
		#Bhvr	Behaviors/Spin2.bhvr
		LifeSpan 60
	End

	Event
		Ename	PushForce
		Type	Local

		At	OverHead
		AltPiv	1

		BhvrOverride
			PositionOffset		0 0 7.5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		20
			PhysForcePower		200
			PhysForcePowerJitter	100
		End


		LifeSpan	75

	End


	Event
		Ename	PushForce2
		Type	Local

		At	OverHead
		AltPiv	1

		BhvrOverride
			PositionOffset		0 0 37.5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		20
			PhysForcePower		100
			PhysForcePowerJitter	50
		End


		LifeSpan	75

	End
End



End