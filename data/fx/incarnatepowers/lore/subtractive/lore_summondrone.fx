#############################################################
## CostumeChange_Tech.fx
#############################################################

FxInfo
LifeSpan 120

## SIGIL #######################################################

Condition
	On	Time
	Time	10

	Event
		Type	Start
		At	Root
		part	:Lore_LightningFloor.part
		Lifespan 20
		LifespanJitter 0
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	Incarnate_Lore_Summon_01 100 100 .8
	End

	Event
		Type	Start
		At	Root
		part	:Lore_Dust.part
		Lifespan 15
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Start
		At	hips
		BhvrOverride

			PositionOffset 0.0 0.5 0.0

		End

		part	:Lore_LightningInsideDrone.part
		Lifespan 20
		LifespanJitter 0
	End
End
Condition
	On	Time
	Time	10

	Event
		Type	Start
		At	hips
		BhvrOverride

			PositionOffset 0.0 -5 0.0

		End
		part	:Lore_FloorGlowDrone.part
		Lifespan 20
		LifespanJitter 0
	End
End
Condition
	On	Time
	Time	10

	Event
		Type	Start
		At	hips
		BhvrOverride

			PositionOffset 0.0 -6.5 0.0

		End


		part	:Lore_FloorGlowDroneB.part
		Lifespan 20
		LifespanJitter 0
	End
End
Condition
	On	Time
	Time	10

	Event
		Type	Start
		At	hips
		BhvrOverride

			PositionOffset 0.0 -5 0.0

		End

		part	:Lore_MiddleGlowDrone.part
		Lifespan 20
		LifespanJitter 0
	End
End
Condition
	On	Time
	Time	40

	Event
		Type	Start
		At	hips
		BhvrOverride

			PositionOffset 0.0 0 0.0

		End

		part	:Lore_BammGlow.part
		part	:Lore_Sparks.part
		Lifespan 1
		LifespanJitter 0
	End
End
Condition
	On	Time
	Time	40

	Event
		Type	Start
		At	hips
		BhvrOverride

			PositionOffset 0.0 0 0.0

		End

		part	:Lore_LightningEnd.part
		part	:Lore_LightningEndB.part
		Lifespan 20
		LifespanJitter 0
	End
End


Condition
	On	Time
	Time	8
	Event
		Type	Start
		At	hips
		Bhvr	behaviors/GenericGeometryFadeBDrone.bhvr
		BhvrOverride
			PositionOffset 0.0 -1 0.0
			FadeOutLength		25
			PyrRotate 		0.0 0.0 0.0
			PyrRotateJitter 	0.0 360.0 0.0
			Alpha 64
			TintGeom	1
		End

		Geom	Lore_HullSphere_Dark

		Lifespan 25
		LifespanJitter	0
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Start
		At	hips
		BhvrOverride

			PositionOffset 0.0 1 0.0
		End
		Part	:Lore_RingsDrone.part
		Lifespan 18
	End

End

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	hips
		part	:Lore_BodyGlowDrone.part
		Lifespan 30
	End
End
Condition
	On	Time
	Time	50

	Event
		Type	Start
		At	hips
		part	:Lore_LightningBodyDrone.part
		Lifespan 20
		LifespanJitter 0
	End
End

#############################################################




End