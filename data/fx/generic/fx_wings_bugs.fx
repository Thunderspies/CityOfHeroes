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
		Anim	FX_wings_bugs
		
#		BhvrOverride
#			Alpha	55
#		End
	End
End


Condition	
	On 		TriggerBits
	TriggerBits	FLY
	DoMany		1

	Event
		EName	blur
		Type	Local
		At	Back
		While	FLY
		Flags	OneAtATime
		ChildFX	generic/FX_wings_Bugs_blur.fx
		
#		BhvrOverride
#			Alpha	55
#		End

	End
End


Condition	
	On 		TriggerBits
	TriggerBits	JUMP
	DoMany		1

	Event
		EName	blur
		Type	Local
		At	Back
		While	JUMP
		Flags	OneAtATime
		ChildFX	generic/FX_wings_Bugs_blur.fx
		
#		BhvrOverride
#			Alpha	55
#		End

	End
End


End