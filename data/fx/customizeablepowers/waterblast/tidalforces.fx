#
FxInfo

	LifeSpan 75


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	WB_TidalForces 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At WepR
			Type Local
			EName mainAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterShot_HandGlow.part
			Part :WaterBurst_DrawIn_Spray3.part
			Part :WaterShot_GlowStreaks3.part
			Part :WaterBall_DrawIn_Splatter3.part
			Part :WaterBall_Drips.part
			LifeSpan 60
		End

		Event
			At WepL
			Type Local
			EName mainAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterShot_HandGlow.part
			Part :WaterBurst_DrawIn_Spray3.part
			Part :WaterShot_GlowStreaks3.part
			Part :WaterBall_DrawIn_Splatter3.part
			Part :WaterBall_Drips.part
			LifeSpan 60
		End

		Event
			At WepR
			Type PositOnly
			EName mainAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Drips_Hand.part
		End

		Event
			At WepL
			Type PositOnly
			EName mainAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Drips_Hand.part
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Sound aim2 60 60 0.6
		End

		Event
			At Chest
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part Powers\YellowUpGlow.part
			Part Powers\YellowUpSpecks2.part
			LifeSpan 13
		End

	End

	Condition
		On Time
		Time 10

		Event
			At Chest
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part Powers\YellowUpRing01.part
			Part Powers\YellowUpRing01a.part
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Chest
			Type Local
			EName Pow
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part Powers\YellowUpRing02.part
			Part Powers\YellowUpRing02a.part
		End

	End

	Condition
		On Time
		Time 30

		Event
			At Chest
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part Powers\YellowUpRingSlow.part
			Part Powers\YellowUpRing03.part
			Part Powers\YellowUpRing03a.part
		End

	End

End
