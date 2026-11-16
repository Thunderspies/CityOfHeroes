#                                                          
FxInfo

	LifeSpan 70


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Sound EMPLuminary 90 90 0.9
		End

	End

	Condition
		On Time
		Time 18

		Event
			At Chest
			Type Local
			EName Mallet1
			Bhvr :EMPBubbleScale.bhvr
			Geom Tintable_ElectricalMeleeBubble
			LifeSpan 5
		End

		Event
			At Chest
			Type Start
			EName Mallet2
			Part :Dots.part
			Sound arrowhitEMP 80 70 0.8
			LifeSpan 10
		End

		Event
			At Chest
			Type Start
			EName Mallet2
			Part :HitEmber.part
			Part :HitStar.part
			#Part :HitEmberFlat.part
			#Part :HitStarFlat.part
			Part :HitRing.part
			Part :HitRing2.part
			Part :HitGlows2.part
			#Sound PunchHit2 100.0 30.0 0.8
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Chest
			Type Start
			EName Mallet2
			Part :EMPElectricityArch1.part
			Part :EMPElectricityArch2.part
			LifeSpan 8
		End

	End

	Condition
		On Time
		Time 25

		Event
			Type Destroy
			EName Mallet1
		End

	End

End
