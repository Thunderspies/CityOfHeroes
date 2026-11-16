#########################################################
## FX Geo - Right Claws - 04
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Bhvr	Behaviors\InheritGroupTint.bhvr
		Geom	GEO_WepR_Vanguard_Blade
	End

	Event
		Type	Local
		At	Origin
		Bhvr	Behaviors\InheritGroupTint.bhvr
		BhvrOverride
			PositionOffset	0 0.375 0
			PyrRotate	0 0 90
		End
		#Part	:TalsorianBlades_Glow01.part
	End
End

#########################################################

End