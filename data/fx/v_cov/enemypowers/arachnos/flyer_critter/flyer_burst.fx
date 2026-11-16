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
		Sound ArachnosflyerShot4 200 200 .94
		
	End
End


Condition
	On 	Time
	Time 	15
	

	Event
		Type	positonly
		At 	C_Rwing1
		childFX :flyer_1Shot_Target.fx
		lifespan 80
		bhvroverride
			stretch 1
		end
		lookat target
	End


end
Condition
	On 	Time
	Time 	20
	

	Event
		Type	positonly
		At 	C_Lwing1
		childFX :flyer_1Shot_Target.fx
		lifespan 80
		bhvroverride
			stretch 1
		end
		lookat target
	End

end
Condition
	On 	Time
	Time 	25
	

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
		At 	C_Rwing2
		childFX :flyer_1Shot_Target.fx
		lifespan 80
		bhvroverride
			stretch 1
		end
		lookat target
	End

end
Condition
	On 	Time
	Time 	30
	

	Event
		Type	positonly
		At 	C_Lwing2
		childFX :flyer_1Shot_Target.fx
		lifespan 80
		bhvroverride
			stretch 1
		end
		lookat target
	End

end