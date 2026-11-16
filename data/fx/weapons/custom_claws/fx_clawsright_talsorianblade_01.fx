#########################################################
## FX Geo - Talsorian Blade Right
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	Local
#		At	Origin
#		Geom	GEO_WepR_Vanguard_Blade
#	End
#
#	Event
#		Type	Local
#		At	Origin
#		BhvrOverride
#			PositionOffset	0 0.375 0
#			PyrRotate	0 0 90
#		End
#		Part	:TalsorianBlades_Glow01.part
#	End
#End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	TalsorianBlades_Glow
		Type	Local
		At	Origin
		Bhvr	:TalsorianBlades_Fade.bhvr
		BhvrOverride
			PositionOffset	0 0.375 0
			PyrRotate	0 0 90
		End
		Part	:TalsorianBlades_Glow01.part
		POther	BladeEndOffset
	End
End

#########################################################

End