#############################################################
## Child_FieldFlashes.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Cycle
	Time 	5
	Chance	0.375

	Event
		Type	Local
		At	Root
		Bhvr	:QuickFade.bhvr
		BhvrOverride
			StartColor	255 255 240
			Scale		7 14.5 7
			Alpha		160
		End
		Geom	GEO_WhiteCylinder
		Lifespan 1
	End
End

#############################################################

End