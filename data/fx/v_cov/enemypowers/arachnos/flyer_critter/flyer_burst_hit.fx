#########################################################
##	
##

FxInfo
LifeSpan  60

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

##################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	chest	
		Sound ArachnosflyerShotHit 90 90 .98
		Sound ArachnosflyerShotHit2 90 90 .98
		
	End
End


Condition
	On 	time
	Time  	0

Event
		ename   Chestx
		Type	positonly
		At	chest
		Part1	:flyer_blastsparks.Part
		lifespan 45
		lookat T_Root
		pmagnet	T_chest
end

end
Condition
	On 	time
	Time  	3

Event
		ename   Chestx
		Type	positonly
		At	chest
		Part1	:flyer_blastsparks.Part
		lifespan 45
		lookat T_Root
		pmagnet	T_chest
end

end
Condition
	On 	time
	Time  	6

Event
		ename   Chestx
		Type	positonly
		At	chest
		Part1	:flyer_blastsparks.Part
		lifespan 45
		lookat T_Root
		pmagnet	T_chest
end

end
Condition
	On 	time
	Time  	9

Event
		ename   Chestx
		Type	positonly
		At	chest
		Part1	:flyer_blastsparks.Part
		lifespan 45
		lookat T_Root
		pmagnet	T_chest
end

end
	
	

#