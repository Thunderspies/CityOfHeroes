#########################################################
##	template

FxInfo

LifeSpan	80

Condition
	On 	Time
	Time 	14

	Event
		Type	local
		At	Origin
		Sound Whooshbig3 100.0 100.0 .8
	End

End

Condition
	On 	Time
	Time 	23

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		Lifespan 1
	End

	Event
		EName	topPoint
		Type	local
		At	WepR
		
		bhvr	behaviors/Katana.bhvr
		Part	:KatanaTrail2.part
		
		
	End

	Event
		EName	Streak
		Type	local
		At	WepR
		
		Part	:KatanaTrail2.part
				
	End

End

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	WepR
		
		Part	:KatanaElectricity.part
		Lifespan	75
	End

End

Condition
	On 	Time
	Time 	17

	
	Event
		
		Type	local
		At	WepR
				
		Part	:KatanaBright2.part
		Part	:KatanaBright2.part

		bhvr	behaviors/Katanabanner.bhvr
		Lifespan	6
	End

	Event
		
		Type	local
		At	WepR
		
		Part	:KatanaBright2.part	
		Part	:KatanaBright2.part
		bhvr	behaviors/KatanabannerBottem.bhvr
		Lifespan	6
	End


	Event
		
		Type	local
		At	WepR

		Part	:KatanaBright2.part		
		Part	:KatanaBright2.part
		bhvr	behaviors/Katanabannertop.bhvr
		Lifespan	6
	End

End
	
End	

	