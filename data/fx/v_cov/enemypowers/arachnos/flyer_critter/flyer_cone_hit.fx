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
	On 	cycle
	Time  	6
	chance .5
Event
		ename   Chestx
		Type	positonly
		At	chest
		Part1	:flyer_blastsparks.Part
		lifespan 45
		lookat T_Root
		pmagnet	T_chest
		Sound ArachnosflyerShotHit 90 90 .98
end

end
	
	

#