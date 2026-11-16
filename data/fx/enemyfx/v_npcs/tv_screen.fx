#############################################################
## TV_Screen.fx
#############################################################

FxInfo

##################################################################

Condition
	On	cycle
	Time	2

	Event
		EName	screen
		Type	local
		At	origin
		Geom	FX_TVscreen_Noise_03
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_02
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_02
		Geom	FX_TVscreen_Noise_03
		Geom	FX_TVscreen_Noise_04
		Geom	FX_TVscreen_Noise_05
		Geom	FX_TVscreen_Noise_06
		Geom	FX_TVscreen_Noise_02
		Geom	FX_TVscreen_Noise_03
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_02
		Geom	FX_TVscreen_Pattern_03
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_02
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_02
		Geom	FX_TVscreen_Pattern_02
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_03
		Geom	FX_TVscreen_Pattern_03
		Geom	FX_TVscreen_Pattern_02
		Geom	FX_TVscreen_Pattern_02
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_03
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_02
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_02
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Noise_04
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_02
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_06
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_02
		Geom	FX_TVscreen_Noise_04
		Geom	FX_TVscreen_Noise_02
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_02
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Pattern_03
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_01
		Geom	FX_TVscreen_Pattern_02
		Geom	FX_TVscreen_Pattern_02
		Geom	FX_TVscreen_Pattern_02
		Geom	FX_TVscreen_Pattern_02
		Geom	FX_TVscreen_Pattern_03
		Geom	FX_TVscreen_Pattern_03
		Geom	FX_TVscreen_Pattern_02
		Geom	FX_TVscreen_Noise_03
		Geom	FX_TVscreen_Noise_02
		Geom	FX_TVscreen_Noise_03
		Geom	FX_TVscreen_Pattern_02
		Geom	FX_TVscreen_Pattern_03
		Geom	FX_TVscreen_Noise_03
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_06
		Geom	FX_TVscreen_Noise_03
		Geom	FX_TVscreen_Noise_04
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_05
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_03
		LifeSpan 2
	End
End

Condition
	On	cycle
	Time	1

	Event
		EName	screen
		Type	local
		At	origin
		Geom	FX_TVscreen_Noise_03
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_02
		Geom	FX_TVscreen_Noise_03
		Geom	FX_TVscreen_Noise_06
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_05
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_03
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_02
		Geom	FX_TVscreen_Noise_03
		Geom	FX_TVscreen_Noise_04
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_02
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_06
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_02
		Geom	FX_TVscreen_Noise_04
		Geom	FX_TVscreen_Noise_02
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_02
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_01
		Geom	FX_TVscreen_Noise_03
		LifeSpan 1
	End
End

Condition
	On	cycle
	Time	100
	Chance 0.05

	Event
		EName	screen
		Type	local
		At	origin
		Geom	FX_TVscreen_Arachnos_02
		Geom	FX_TVscreen_Arachnos_02
		Geom	FX_TVscreen_Arachnos_02
		LifeSpan 50
	End
End

Condition
	On	cycle
	Time	100
	Chance 0.25

	Event
		EName	screen
		Type	local
		At	origin
		Geom	FX_TVscreen_Pattern_04
		LifeSpan 50
	End
End

#############################################################

End