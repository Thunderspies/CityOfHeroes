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

		part3	:P_GlowCore_RecluseHandCone.part
#		Geom	Testing_TargetB
	End
	
End

Condition
	On	Time
	Time	3

	Event
		EName	Glow
		Type 	Local
		At	Origin
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseHandCone.part
		part2	:P_GlowSparks_RecluseHandCone.part
	End

	Event
		EName	Sparls
		Type 	Local
		At	Origin
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseFingersCone.part
	End

End

Condition
	On	Time
	Time	7
	
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

	