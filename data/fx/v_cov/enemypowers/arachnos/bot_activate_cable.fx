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
		ename	targetz
		Type	local
		At 	origin
		bhvroverride
			positionoffset 0 0 1
		end
	End

	Event
		ename	cableshoot
		Type	startpositonly
		At 	targetz
		magnet origin
		bhvroverride
			trackrate	2
		end
	End
	Event
		ename	targeta
		Type	positonly
		At 	cableshoot
		bhvroverride
			stretch		1
		end
		lookat	targetz
	End
	Event
		Type	positonly
		At 	cableshoot
		lookat	targetz
		part	:bot_activate_cable_elecsmoke.part
		part	:bot_activate_cable_elecsmoke.part
		part	:bot_activate_cable_elecsmoke.part
		lifespan	10
		bhvroverride
			scale	.2 .2 .2
		end
	End
	Event
		ename	prime2
		Type	startpositonly
		At 	targetz
		magnet origin
#		geom	testing_target
		bhvroverride
			positionoffset	0 2 0 
			startjitter	0 20 0
			trackrate	2
		end
	End
	Event
		ename	ropecenter
		Type	local
		At 	targeta
		bhvr	:bot_activate_cablesag.bhvr
		bhvroverride
			positionoffset 0 0 .5
#			scale	5 5 -1
		end		
	End
	Event
		ename	ropespot0
		Type	local
		At 	ropecenter
#		geom	testing_target
		bhvroverride
			positionoffset 0 0 .5
		end		
	End
	Event
		ename	ropespot1
		Type	local
		At 	ropecenter
		bhvroverride
			positionoffset 0 -.18 .4
		end		
	End
	Event
		ename	ropespot2
		Type	local
		At 	ropecenter
		bhvroverride
			positionoffset 0 -.32 .3
		end		
	End
	Event
		ename	ropespot3
		Type	local
		At 	ropecenter
		
		bhvroverride
			positionoffset 0 -.42 .2
		end		
	End
	Event
		ename	ropespot4
		Type	local
		At 	ropecenter
		
		bhvroverride
			positionoffset 0 -.48 .1
		end		
	End
	Event
		ename	ropespot5
		Type	local
		At 	ropecenter
		
		bhvroverride
			positionoffset 0 -.5 0
		end		
	End
	Event
		ename	ropespot6
		Type	local
		At 	ropecenter
		
		bhvroverride
			positionoffset 0 -.48 -.1
		end		
	End
	Event
		ename	ropespot7
		Type	local
		At 	ropecenter
		
		bhvroverride
			positionoffset 0 -.42 -.2
		end
	End
	Event
		ename	ropespot8
		Type	local
		At 	ropecenter
		
		bhvroverride
			positionoffset 0 -.32 -.3
		end
	End
	Event
		ename	ropespot9
		Type	local
		At 	ropecenter
		
		bhvroverride
			positionoffset 0 -.18 -.4
		end
	End	
	Event
		ename	ropespot10
		Type	local
		At 	ropecenter
		
		bhvroverride
			positionoffset 0 0 -.5
		end
	End	

End


Condition
	On 	Time
	Time 	0

	Event
		ename	rope1
		pother	targeta
		Type	positonly
		At 	ropespot1
		part	:bot_activate_cable.part
		part	:bot_activate_cable_redhot.part
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope2
		pother	rope1
		Type	positonly
		At 	ropespot2
		part	:bot_activate_cable.part
		part	:bot_activate_cable_redhot.part
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope3
		pother	rope2
		Type	positonly
		At 	ropespot3
		part	:bot_activate_cable.part
		part	:bot_activate_cable_redhot.part
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope4
		pother	rope3
		Type	positonly
		At 	ropespot4
		part	:bot_activate_cable.part
		part	:bot_activate_cable_redhot.part
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope5
		pother	rope4
		Type	positonly
		At 	ropespot5
		part	:bot_activate_cable.part
		part	:bot_activate_cable_redhot.part
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope6
		pother	rope5
		Type	positonly
		At 	ropespot6
		part	:bot_activate_cable.part
		part	:bot_activate_cable_redhot.part
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope7
		pother	rope6
		Type	positonly
		At 	ropespot7
		part	:bot_activate_cable.part
		part	:bot_activate_cable_redhot.part
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope8
		pother	rope7
		Type	positonly
		At 	ropespot8
		part	:bot_activate_cable.part
		part	:bot_activate_cable_redhot.part
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope9
		pother	rope8
		Type	positonly
		At 	ropespot9
		part	:bot_activate_cable.part
		part	:bot_activate_cable_redhot.part
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope10
		pother	rope9
		Type	positonly
		At 	ropespot10
		part	:bot_activate_cable.part
		part	:bot_activate_cable_redhot.part
		part	:bot_activate_cablelite.part
	End

end



Condition
	On 	prime2hit
#	Time 	0

	Event
		ename	rope1
		pother	targeta
		Type	positonly
		At 	ropespot1

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		ename	rope2
		pother	rope1
		Type	positonly
		At 	ropespot2

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		ename	rope3
		pother	rope2
		Type	positonly
		At 	ropespot3

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		ename	rope4
		pother	rope3
		Type	positonly
		At 	ropespot4

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		ename	rope5
		pother	rope4
		Type	positonly
		At 	ropespot5

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		ename	rope6
		pother	rope5
		Type	positonly
		At 	ropespot6

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		ename	rope7
		pother	rope6
		Type	positonly
		At 	ropespot7

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		ename	rope8
		pother	rope7
		Type	positonly
		At 	ropespot8

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		ename	rope9
		pother	rope8
		Type	positonly
		At 	ropespot9

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		ename	rope10
		pother	rope9
		Type	positonly
		At 	ropespot10

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End

end