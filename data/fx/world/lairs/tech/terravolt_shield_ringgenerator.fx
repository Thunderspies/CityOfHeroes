#########################################################
##	tech_rings1
##
FxInfo
	
LifeSpan 300

Input  
	InpName	Origin
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound glowroom_loop 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound beeprise2m_loop 100 100 .36
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Origin
		Part1	world/Lairs/tech/TerraVolt_Shield_RingGenerator.part
		Sound energycore4_loop 100 100 .33
		
	End
End

End			