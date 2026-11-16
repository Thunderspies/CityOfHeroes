#########################################################
##	template

FxInfo

Flags InheritAlpha ###InheritAnimScale

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	Back
		Anim	FX_wings_DEMON
		
		Bhvr	behaviors/WingScale_DEMON_Male.bhvr
	End
End

End