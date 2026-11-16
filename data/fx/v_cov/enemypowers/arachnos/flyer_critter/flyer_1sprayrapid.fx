#########################################################
##	
##

FxInfo
LifeSpan  80

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
	Time 	4
	

	Event
		ename	rootspot1
		Type	local
		At 	t_root
		lifespan 3
#		geom	testing_target
		bhvroverride
			startjitter 7 5 7
		end
	End
	Event
		Type	positonly
		At 	origin
		childFX :flyer_1Shot_Target.fx
		lifespan 80
		bhvroverride
			stretch 1
		end
		lookat rootspot1
	End


end

