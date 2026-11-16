#                                                          
FxInfo

	Flags InheritAlpha DontSuppress


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			EName OriginOffset
			Bhvr behaviors\GenericParticleFade.bhvr
			Part :Dots.part
			Part :Stars.Part
			Part V_COV\Items\VoidSkiff\Twinkle.part
			Part V_COV\Items\VoidSkiff\SparkleRing.part
			Part V_COV\Items\VoidSkiff\Flashes.part
			Part V_COV\Items\VoidSkiff\Flashes_Trail.part
			Part V_COV\Items\VoidSkiff\OuterRing_Main.part
		End

	End

	Condition
		On Cycle
		Time 30

		Event
			At OriginOffset
			Type Local
			PMagnet OriginOffset
			Bhvr behaviors\GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.275 0
			End
			Part :EnergyShield_ExpandingRings1.part
			LifeSpan 1
		End

	End

	Condition
		On Cycle
		Time 12

		Event
			At OriginOffset
			Type Local
			PMagnet OriginOffset
			Bhvr behaviors\GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.275 0
			End
			Part :EnergyShield_ExpandingRings2.part
			LifeSpan 1
		End

	End

End
