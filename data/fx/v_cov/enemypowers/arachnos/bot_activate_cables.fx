#########################################################
##	
##

FxInfo
#LifeSpan  1000

Input  
	InpName	target
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
		ename	behindspot
		Type	local
		At 	root
		bhvroverride
			positionoffset 0 0 10
		end
	End
	Event
		ename	behind
		Type	start
		At 	behindspot
	End
	Event
		ename	aligner
		Type	startpositonly
		At 	root
		lookat	behind
	End
	Event
		ename	targets
		Type	local
		At 	aligner
		#geom	testing_target
		bhvroverride
			positionoffset 0 9.5 -21.5
			scale	1	.7	1
		end
	End
	end
Condition
	On 	Time
	Time 	0
#########
	Event
		ename	attach1
		Type	local
		At 	hair
		#geom	testing_target
		bhvroverride
			positionoffset 1.6 4 -2.5
		end
	End
	Event
		ename	target1
		Type	local
		At 	targets
		#geom	testing_target
		bhvroverride
			positionoffset  0 2.5 0
		end
	End

	Event
		ename	rope1
		Type	positonly
		At 	attach1
		lookat target1
		bhvroverride
			stretch 1
		end
		childFX	:bot_activate_cable.fx
	End


	end
Condition
	On 	Time
	Time 	5
#########
	Event
		ename	attach2
		Type	local
		At 	hair
		#geom	testing_target
		bhvroverride
			positionoffset -1.6 4 -2.5
		end
	End
	Event
		ename	target2
		Type	local
		At 	targets
		#geom	testing_target
		bhvroverride
			positionoffset  0 1.5 0
		end
	End

	Event
		ename	rope2
		Type	positonly
		At 	attach2
		lookat target2
		bhvroverride
			stretch 1
		end
		childFX	:bot_activate_cable.fx
	End
	end
Condition
	On 	Time
	Time 	10
#########
	Event
		ename	attach3
		Type	local
		At 	hair
		#geom	testing_target
		bhvroverride
			positionoffset 1.8 2 -2.8
		end
	End
	Event
		ename	target3
		Type	local
		At 	targets
		#geom	testing_target
		bhvroverride
			positionoffset  0 .5 0
		end
	End

	Event
		ename	rope3
		Type	positonly
		At 	attach3
		lookat target3
		bhvroverride
			stretch 1
		end
		childFX	:bot_activate_cable.fx
	End
	end
Condition
	On 	Time
	Time 	15
#########
	Event
		ename	attach4
		Type	local
		At 	hair
		#geom	testing_target
		bhvroverride
			positionoffset -1.8 2 -2.8
		end
	End
	Event
		ename	target4
		Type	local
		At 	targets
		#geom	testing_target
		bhvroverride
			positionoffset  0 -.5 0
		end
	End

	Event
		ename	rope4
		Type	positonly
		At 	attach4
		lookat target4
		bhvroverride
			stretch 1
		end
		childFX	:bot_activate_cable.fx
	End
	end
Condition
	On 	Time
	Time 	20
#########
	Event
		ename	attach5
		Type	local
		At 	hair
		#geom	testing_target
		bhvroverride
			positionoffset 1.2 .5 -2
		end
	End
	Event
		ename	target5
		Type	local
		At 	targets
		#geom	testing_target
		bhvroverride
			positionoffset  0 -1.5 0
		end
	End

	Event
		ename	rope5
		Type	positonly
		At 	attach5
		lookat target5
		bhvroverride
			stretch 1
		end
		childFX	:bot_activate_cable.fx
	End
	end
Condition
	On 	Time
	Time 	25
#########
	Event
		ename	attach6
		Type	local
		At 	hair
		#geom	testing_target
		bhvroverride
			positionoffset -1.2 .5 -2
		end
	End
	Event
		ename	target6
		Type	local
		At 	targets
		#geom	testing_target
		bhvroverride
			positionoffset  0 -2.5 0
		end
	End

	Event
		ename	rope6
		Type	positonly
		At 	attach6
		lookat target6
		bhvroverride
			stretch 1
		end
		childFX	:bot_activate_cable.fx
	End


end