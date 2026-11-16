#########################################################
##	Shazam Orbit Child
##
FxInfo

#LifeSpan	200

Condition
	On 	Time
	Time 	0

	Event
		EName 	Center
		Type	Local
		At	Root
		BhvrOverride
			Spin	0 0.7 0
		End
	End
	
	Event
		EName 	Spinner
		Type	Local
		At	Center
		BhvrOverride
			PositionOffset	0 4 4
			Spin	4.3 0 0
		End
	End
	
	Event
		EName 	Ring
		Type	Local
		At	Spinner
		BhvrOverride
			PositionOffset	0 0 .1
		End
		Part	:SonicPulse.part
	End
	
	Event
		EName 	Center2
		Type	Local
		At	Root
		BhvrOverride
			Spin	0 -0.66 0
		End
	End
		
	Event
		EName 	Spinner2
		Type	Local
		At	Center2
		BhvrOverride
			PositionOffset	0 4 4
			Spin	4.1 0 0
		End
	End
	
	Event
		EName 	Ring2
		Type	Local
		At	Spinner2
		BhvrOverride
			PositionOffset	0 0 .1
		End
		Part	:SonicPulse.part
	End
End

End	


			