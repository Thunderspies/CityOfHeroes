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
	Time 	17

	Event
		Type	Local
		At	wepR	
		Sound ArachnosflyerShot 200 200 .94
		
	End
End




Condition
	On 	Time
	Time 	18
	

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
	Time 	19
	

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
	Time 	20
	

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
	Time 	21
	

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

Condition
	On 	Time
	Time 	26
	
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
	Time 	27
	

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
	Time 	28
	
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
	Time 	29
	

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


#Condition
#	On 	Time
#	Time 	38
#	
#
#	Event
#		Type	positonly
#		At 	C_Rwing1
#		childFX :flyer_1Shot_Target.fx
#		lifespan 80
#		bhvroverride
#			stretch 1
#		end
#		lookat target
#	End
#
#
#end
#Condition
#	On 	Time
#	Time 	39
#	
#
#	Event
#		Type	positonly
#		At 	C_Lwing1
#		childFX :flyer_1Shot_Target.fx
#		lifespan 80
#		bhvroverride
#			stretch 1
#		end
#		lookat target
#	End
#
#end
#Condition
#	On 	Time
#	Time 	40
#	
#	Event
#		Type	positonly
#		At 	C_Rwing2
#		childFX :flyer_1Shot_Target.fx
#		lifespan 80
#		bhvroverride
#			stretch 1
#		end
#		lookat target
#	End
#
#end
#Condition
#	On 	Time
#	Time 	41
#	
#
#	Event
#		Type	positonly
#		At 	C_Lwing2
#		childFX :flyer_1Shot_Target.fx
#		lifespan 80
#		bhvroverride
#			stretch 1
#		end
#		lookat target
#	End
#
#end