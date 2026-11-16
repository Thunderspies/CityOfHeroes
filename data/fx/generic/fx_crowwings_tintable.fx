#############################################################
## FX_CrowWings.fx
#############################################################

FxInfo
Flags InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		Anim	FX_CrowWings_Tintable
		BhvrOverride
			TintGeom	1
		End
	End
End

#############################################################

End