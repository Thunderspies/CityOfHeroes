#########################################################
##	
##

FxInfo
LifeSpan  100

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
	Time 	15

	Event
		Type	Local
		At	wepR	
		Sound ArachnosflyerShotCone2 200 200 .94
		
	End
End


Condition
	On 	time
	Time 	15
	
	Event
		Type	positonly
		At 	C_Rwing1
		childFX :flyer_1SprayRapid.fx
		lifespan 80
	End


end

Condition
	On 	time
	Time 	16
	
	Event
		Type	positonly
		At 	C_Lwing1
		childFX :flyer_1SprayRapid.fx
		lifespan 80
	End


end

Condition
	On 	time
	Time 	17
	
	Event
		Type	positonly
		At 	C_Rwing2
		childFX :flyer_1SprayRapid.fx
		lifespan 80
	End


end

Condition
	On 	time
	Time 	18
	

	Event
		ename	prime
		Type	start
		At 	C_Rwing1
#########		geom	testing_target
		bhvroverride
			trackrate 4
		end
		magnet	target
	End

	Event
		Type	positonly
		At 	C_Lwing2
		childFX :flyer_1SprayRapid.fx
		lifespan 80
	End


end

