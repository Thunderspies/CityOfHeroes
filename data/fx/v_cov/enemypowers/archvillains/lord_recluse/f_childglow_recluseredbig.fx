#########################################################
##	template

FxInfo

Lifespan 100

Condition
	On Time 
	Time 0
	
	Event
		EName	Core
		Type 	Local
		At	Origin
		bhvrOverride
			HueShift	15
		End
		
		part3	:P_GlowCore_RecluseHandBig.part
#		Geom	Testing_TargetB
	End
	
End

Condition
	On	Time
	Time	5

	Event
		EName	Glow
		Type 	Local
		At	Origin
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseHandBig.part
		part2	:P_GlowSparks_RecluseHandBig.part
	End
End

Condition
	On	Time
	Time	14
	
	Event
		EName	Trail
		Type	Local
		At	Origin
		bhvrOverride
			HueShift	15
		End
		
		part2	:P_Glow_RecluseHandMove.part
	End
End




End	

	