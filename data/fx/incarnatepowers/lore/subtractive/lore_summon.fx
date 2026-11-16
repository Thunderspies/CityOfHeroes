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
		Lifespan 25
		LifespanJitter 10
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
		At	Root
		BhvrOverride

			PositionOffset 0.0 4 0.0

		End

		part	:Lore_LightningInside.part
		Lifespan 20
		LifespanJitter 0
	End
End
Condition
	On	Time
	Time	30

	Event
		Type	Start
		At	Root
		BhvrOverride

			PositionOffset 0.0 1 0.0

		End

		part	:Lore_Lightning.part
		Lifespan 15
		LifespanJitter 0
	End
End
Condition
	On	Time
	Time	20

	Event
		Type	Start
		At	Root
		BhvrOverride

			PositionOffset 0.0 0 0.0

		End

		part	:Lore_MiddleGlow.part
		Lifespan 20
		LifespanJitter 0
	End
End
Condition
	On	Time
	Time	40

	Event
		Type	Start
		At	Root
		BhvrOverride

			PositionOffset 0.0 5.0 0.0

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
		At	Root
		BhvrOverride

			PositionOffset 0.0 8.0 0.0

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
		At	Root
		Bhvr	behaviors/GenericGeometryFadeB.bhvr
		BhvrOverride
			PositionOffset 		0.0 2.0 0.0
			FadeOutLength		25
			PyrRotate 		0.0 0.0 0.0
			PyrRotateJitter 	0.0 360.0 0.0
			TintGeom		1
			Alpha 			64
		End

		Geom	Lore_HullSphere_dark

		Lifespan 25
		LifespanJitter	0
	End
End
Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Root
		BhvrOverride

			PositionOffset 0.0 4.0 0.0

		End
		Part	:Lore_GroundCircle_Rising.part
		Lifespan 10
	End

End

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	Root
		Part	:Lore_BodyGlow.part
		Part	:Lore_BodyGlow_Lingering.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	50

	Event
		Type	Start
		At	Root
		BhvrOverride

			PositionOffset 0.0 3 0.0

		End
		part	:Lore_LightningBody.part
		Lifespan 50
		LifespanJitter 0
	End
End

#############################################################




End