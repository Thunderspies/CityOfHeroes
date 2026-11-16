###########################################################

FxInfo
Lifespan 60

###########################################################

Condition
	On	Time
	Time	20

	Event
		At Root
		Type Start
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Part	:TerribleHowl_Ring.part
		Part 	:TerribleHowl_Wind_Shockwave.part
		Part 	:TerribleHowl_Wind_Shockwave_Core.part
		LifeSpan 10
	End

	Event
		At Root
		Type Start
		Bhvr	:Dust_Expanding.bhvr
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
			Spin		0.0 -0.1 0.0
		End
		Part	:TerribleHowl_Dust.part
		Part	:TerribleHowl_Snow.part
		Part	:TerribleHowl_Snow_Flakes.part
		LifeSpan 25
	End

	Event
		At Root
		Type Start
		Bhvr	:Geo_Expanding.bhvr
		BhvrOverride
			StartColor	255 255 255
		End
		Geom FX_Tintable_TerribleHowl
		LifeSpan 10
	End

	Event
		At Root
		Type Start
		Bhvr	:Geo_Expanding.bhvr
		BhvrOverride
			PositionOffset 	0.0 -1.0 0.0
			StartColor	255 255 255
		End
		Geom FX_Tintable_TerribleHowl
		LifeSpan 10
	End
End

#Condition
#	On	Time
#	Time	12
#

#End


###########################################################

End