#
FxInfo

	LifeSpan 250

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DD_EarthShatter_Pet_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At T_Root
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 30 0
			End
			Part :EarthShatter_Slime.part
			Part :EarthShatter_Dirt.part
			Part :EarthShatter_Shards.part
			Part :EarthShatter_Ice.part
			#Part :EarthShatter_Tentacles.part
			LifeSpan 240
		End

	End

End
