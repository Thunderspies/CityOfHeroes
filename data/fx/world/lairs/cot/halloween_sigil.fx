#############################################################
## Halloween_Sigil_Spawn.fx
#############################################################

FxInfo
LifeSpan 300

#########################################################

Condition
	On 	Time
	Time	20

	Event
		EName	flash
		Type	StartPositOnly
		At	Hips
		BhvrOverride
			Alpha	150
			StartColor 255 255 0
			Spin	0 .0035 0
			Scale 60 60 60
		End
		Geom	FX_Sigil_TT
	End

#########################################################

End