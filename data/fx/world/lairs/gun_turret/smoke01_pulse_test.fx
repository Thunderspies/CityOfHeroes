#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 80

Condition
	On	Cycle
	Time 	72

	Event

		Type	Local 
		At	Origin
		Part	:gunturret_smoke3.part
		Part	:gunturret_rock.part
		Part	:gunturret_rock2.part
		LifeSpan 80
		
	End
End


Condition
	On	Cycle
	Time 	70

	Event

		Type	Local 
		At	Origin
		Part	:gunturret_smoke.part
		Part	:gunturret_smoke2.part
		#Part	:gunturret_smoke4.part
		LifeSpan 80
		
	End
End			