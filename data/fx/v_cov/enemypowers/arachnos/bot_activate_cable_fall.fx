#########################################################
##	
##

FxInfo
LifeSpan  300

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
		ename	targeta
		Type	local
		At 	origin
	End
	Event
		ename	ropecenter
		Type	local
		At 	targeta
#		
		bhvroverride
			positionoffset 0 0 .5
			scale	5 5 -1
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
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope2
		pother	rope1
		Type	positonly
		At 	ropespot2
		part	:bot_activate_cable.part
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope3
		pother	rope2
		Type	positonly
		At 	ropespot3
		part	:bot_activate_cable.part
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope4
		pother	rope3
		Type	positonly
		At 	ropespot4
		part	:bot_activate_cable.part
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope5
		pother	rope4
		Type	positonly
		At 	ropespot5
		part	:bot_activate_cable.part
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope6
		pother	rope5
		Type	positonly
		At 	ropespot6
		part	:bot_activate_cable.part
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope7
		pother	rope6
		Type	positonly
		At 	ropespot7
		part	:bot_activate_cable.part
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope8
		pother	rope7
		Type	positonly
		At 	ropespot8
		part	:bot_activate_cable.part
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope9
		pother	rope8
		Type	positonly
		At 	ropespot9
		part	:bot_activate_cable.part
		part	:bot_activate_cablelite.part
	End
	Event
		ename	rope10
		pother	rope9
		Type	positonly
		At 	ropespot10
		part	:bot_activate_cable.part
		part	:bot_activate_cablelite.part
	End

	
	Event
		ename	prime2
		Type	start
		At 	origin
		magnet origin
		bhvroverride
			positionoffset	0 1 0 
			startjitter	.3 1 .3
			trackrate	.5
		end
	End
end


Condition
	On 	prime2hit
#	Time 	0

	Event
		#ename	rope1
		pother	targeta
		Type	local
		At 	rope1

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		#ename	rope2
		pother	rope1
		Type	local
		At 	rope2

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		#ename	rope3
		pother	rope2
		Type	local
		At 	rope3

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		#ename	rope4
		pother	rope3
		Type	local
		At 	rope4

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		#ename	rope5
		pother	rope4
		Type	local
		At 	rope5

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		#ename	rope6
		pother	rope5
		Type	local
		At 	rope6

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		#ename	rope7
		pother	rope6
		Type	local
		At 	rope7

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		#ename	rope8
		pother	rope7
		Type	local
		At 	rope8

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		#ename	rope9
		pother	rope8
		Type	local
		At 	rope9

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End
	Event
		#ename	rope10
		pother	rope9
		Type	local
		At 	rope10

		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
	End

end
Condition
	On 	Time
	Time 	120
	Event
		ename	prime
		Type	start
		At 	targetz
		magnet targetz
#		geom	testing_target
		bhvroverride
			positionoffset	0 20 0 
			startjitter	5 20 5
			trackrate	1
		end
	End
end

Condition
	On 	primehit
#	Time 	0


	Event
		ename	rope0
		Type	destroy		
	End
	Event
		ename	rope1
		Type	destroy		
	End
	Event
		ename	rope2
		Type	destroy		
	End
	Event
		ename	rope3
		Type	destroy		
	End
	Event
		ename	rope4
		Type	destroy		
	End
	Event
		ename	rope5
		Type	destroy		
	End
	Event
		ename	rope6
		Type	destroy		
	End
	Event
		ename	rope7
		Type	destroy		
	End
	Event
		ename	rope8
		Type	destroy		
	End
	Event
		ename	rope9
		Type	destroy		
	End
	Event
		ename	rope10
		Type	destroy		
	End
	Event
		ename	rope11
		Type	destroy		
	End
	Event
		ename	dart
		Type	destroy		
	End
	Event
		ename	ropepoint
		Type	destroy		
	End
	Event
		ename	prime2
		Type	destroy		
	End




	Event
		ename	drope0
		Type	start
		At 	ropespot0
		bhvr	:bot_activate_cable_fall.bhvr
		part	:bot_activate_cablelite_fall.part
		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part		
		lifespan	46
		
		
	End


	Event
		pother	drope0
		ename	drope1
		Type	start
		At 	ropespot1
		part	:bot_activate_cable_fall.part
		part	:bot_activate_cablelite_fall.part
		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
		bhvr	:bot_activate_cable_fall.bhvr
		lifespan	46
		
		
	End
	Event
		ename	tipspark
		Type	positonly
		At 	drope0
		part	:bot_activate_cable_elecsmoke.part
		part	:bot_activate_cable_elecspark.part
		lookat	drope1		
		lifespan	46
		
	End
	Event
		ename	socketaim
		Type	local
		At 	hair
#		geom	testing_target
		bhvroverride
			positionoffset 0 7 2
		end
#		lifespan	46
		
	End

	Event
		ename	socketspark
		Type	positonly
		At 	origin
		part	:bot_activate_cable_elecsmoke.part
		part	:bot_activate_cable_elecsmoke.part
		part	:bot_activate_cable_elecsmoke.part
		part	:bot_activate_cable_elecsmoke.part
		part	:bot_activate_cable_elecspark.part
		part	:bot_activate_cable_elecsmoke.part
		part	:bot_activate_cable_elecsmoke.part
		part	:bot_activate_cable_elecspark.part
		lookat	socketaim
		bhvroverride
			scale 1 1 1
		end
		lifespan	10
		
	End

	Event
		pother	drope1
		ename	drope2
		Type	start
		At 	ropespot2
		part	:bot_activate_cable_fall.part
		part	:bot_activate_cablelite_fall.part
		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
		bhvr	:bot_activate_cable_fall.bhvr
		lifespan	46
		
		
	End
	Event
		pother	drope2
		ename	drope3
		Type	start
		At 	ropespot3
		part	:bot_activate_cable_fall.part
		part	:bot_activate_cablelite_fall.part
		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
		bhvr	:bot_activate_cable_fall.bhvr
		lifespan	46
		
		
	End
	Event
		pother	drope3
		ename	drope4
		Type	start
		At 	ropespot4
		part	:bot_activate_cable_fall.part
		part	:bot_activate_cablelite_fall.part
		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
		bhvr	:bot_activate_cable_fall.bhvr
		lifespan	46
		
		
	End
	Event
		pother	drope4
		ename	drope5
		Type	start
		At 	ropespot5
		part	:bot_activate_cable_fall.part
		part	:bot_activate_cablelite_fall.part
		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
		bhvr	:bot_activate_cable_fall.bhvr
		lifespan	46
		
		
	End
	Event
		pother	drope5
		ename	drope6
		Type	start
		At 	ropespot6
		part	:bot_activate_cable_fall.part
		part	:bot_activate_cablelite_fall.part
		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
		bhvr	:bot_activate_cable_fall.bhvr
		lifespan	46
		
		
	End
	Event
		pother	drope6
		ename	drope7
		Type	start
		At 	ropespot7
		part	:bot_activate_cable_fall.part
		part	:bot_activate_cablelite_fall.part
		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
		bhvr	:bot_activate_cable_fall.bhvr
		bhvroverride
			physGravity .9
			gravity	.018
		end
		lifespan	46
		
		
	End
	Event
		pother	drope7
		ename	drope8
		Type	start
		At 	ropespot8
		part	:bot_activate_cable_fall.part
		part	:bot_activate_cablelite_fall.part
		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
		bhvr	:bot_activate_cable_fall.bhvr
		bhvroverride
			physGravity .6
			gravity	.012
		end
		lifespan	46
		
		
	End
	Event
		pother	drope8
		ename	drope9
		Type	start
		At 	ropespot9
		part	:bot_activate_cable_fall.part
		part	:bot_activate_cablelite_fall.part
		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
		bhvr	:bot_activate_cable_fall.bhvr 
		bhvroverride
			physGravity .3
			gravity	.006
		end
		lifespan	46
		
		
	End
	Event
		pother	drope9
		ename	drope10
		Type	start
		At 	ropespot10
		part	:bot_activate_cable_fall.part
		part	:bot_activate_cablelite_fall.part
		part	:bot_activate_elec1.part
		part	:bot_activate_elec2.part
#		bhvr	:bot_activate_cable_fall.bhvr
		lifespan	46
	End


end
