#########################################################
##	
##

FxInfo
LifeSpan  1000

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
		Type	Local
		At	origin	
		Sound ArachnosHarpoon2b 180 180 .72
		
	End
End


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
		ename	dartfacer
		Type	start
		At 	origin
#		geom	testing_target
		bhvroverride
			trackrate	1.5
			initialvelocity 0 1.5 0
			positionoffset	0 2 0
		end
		magnet targetz
	End

	Event
		ename	ropecenter
		Type	local
		At 	origin
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
		ename	prime
		Type	startpositonly
		At 	origin
		magnet	targetz
		bhvroverride
			initialvelocity	0 3.5 0
			gravity	.1
			trackrate 2.8
		end
	End	
	Event
		ename	dartaim
		Type	positonly
		At 	prime
		lookat	dartfacer
	End
	Event
		ename	dart
		Type	local
		At 	dartaim
		geom	FX_tech_dart
		bhvroverride
			scale 10 10 10
			Rgb0			60 30 150
			pyrrotate	180 0 0
			positionoffset	0 0 -2.3
		end
	End

	Event
		ename	ropepoint
		Type	local
		At 	dart
#		geom	testing_target
		part	:Flyer_Beacon_Blink.part
		bhvroverride
			positionoffset	0 0 -.29
			scale	.1 .1 .1
		end

	End

	Event
		ename	contrail
		Type	local
		At 	ropepoint
		part	:flyer_HarpoonContrail.part
	End



	Event
		ename	rope1
		Type	positonly
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		pother	origin
		
	End

	Event
		Type	local
		At 	ropepoint
		part	:flyer_missile_Flash1.part
		part	:flyer_missile_smoke.part
		lifespan 5
		
	End
		

End

Condition
	On 	Time
	Time 	4
	Event
		EName 	rope1
		Type	Destroy
	End

	Event
		ename	rope1
		Type	startpositonly
		At 	ropepoint
		pother	origin
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		bhvroverride
			trackrate .4
		end
		magnet	ropespot1
		
	End
	Event
		ename	rope2
		pother	rope1
		Type	positonly
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		
	End

end

Condition
	On 	Time
	Time 	8
	Event
		EName 	rope2
		Type	Destroy
	End

	Event
		ename	rope2
		Type	startpositonly
		At 	ropepoint
		pother	rope1
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		bhvroverride
			trackrate .6
		end
		magnet	ropespot2
		
	End
	Event
		pother	rope2
		ename	rope3
		Type	positonly
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		
	End

end

Condition
	On 	Time
	Time 	12
	Event
		EName 	rope3
		Type	Destroy
	End

	Event
		pother	rope2
		ename	rope3
		Type	startpositonly
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		bhvroverride
			trackrate .7
		end
		magnet	ropespot3
		
	End
	Event
		pother	rope3
		ename	rope4
		Type	positonly
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		
	End

end

Condition
	On 	Time
	Time 	16
	Event
		EName 	rope4
		Type	Destroy
	End

	Event
		pother	rope3
		ename	rope4
		Type	startpositonly
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		bhvroverride
			trackrate .8
		end
		magnet	ropespot4
		
	End
	Event
		pother	rope4
		ename	rope5
		Type	positonly
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		
	End

end

Condition
	On 	Time
	Time 	20
	Event
		EName 	rope5
		Type	Destroy
	End

	Event
		pother	rope4
		ename	rope5
		Type	startpositonly
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		bhvroverride
			trackrate .9
		end
		magnet	ropespot5
		
	End
	Event
		pother	rope5
		ename	rope6
		Type	positonly
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		
	End

end

Condition
	On 	Time
	Time 	24
	Event
		EName 	rope6
		Type	Destroy
	End

	Event
		pother	rope5
		ename	rope6
		Type	startpositonly
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		bhvroverride
			trackrate 1
		end
		magnet	ropespot6
		
	End
	Event
		pother	rope6
		ename	rope7
		Type	positonly
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		
	End

end

Condition
	On 	Time
	Time 	28
	Event
		EName 	rope7
		Type	Destroy
	End

	Event
		pother	rope6
		ename	rope7
		Type	startpositonly
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		bhvroverride
			trackrate 1.1
		end
		magnet	ropespot7
		
	End
	Event
		pother	rope7
		ename	rope8
		Type	positonly
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		
	End

end

Condition
	On 	Time
	Time 	32
	Event
		EName 	rope8
		Type	Destroy
	End

	Event
		pother	rope7
		ename	rope8
		Type	startpositonly
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		bhvroverride
			trackrate 1.2
		end
		magnet	ropespot8
		
	End
	Event
		pother	rope8
		ename	rope9
		Type	positonly
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		
	End

end

Condition
	On 	Time
	Time 	36
	Event
		EName 	rope9
		Type	Destroy
	End

	Event
		pother	rope8
		ename	rope9
		Type	start
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		bhvroverride
			trackrate 1.3
			positionoffset .1 .1 .1
		end
		magnet	ropespot9
		
	End
	Event
		pother	rope9
		ename	rope10
		Type	positonly
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		
	End

end

Condition
	On 	Time
	Time 	40
	Event
		EName 	rope10
		Type	Destroy
	End

	Event
		pother	rope9
		ename	rope10
		Type	start
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		bhvroverride
			trackrate 1.4
			positionoffset .1 .1 .1
		end
		magnet	ropespot10
		
	End
	Event
		pother	rope10
		ename	rope11
		Type	positonly
		At 	ropepoint
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		
	End

end

Condition
	On 	primehit
	Event
		EName 	rope11
		Type	Destroy
	End
	Event
		EName 	contrail
		Type	Destroy
	End


	Event
		pother	rope10
		ename	rope11
		Type	positonly
		At 	targetz
		part	:flyer_deploy_cable.part
		part	:flyer_deploy_cableflash.part
		
	End

end

Condition
	On 	time
	Time 	190


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
		ename	prime1
		Type	destroy		
	End


	Event
		Type	local
		At 	dartaim
		geom	FX_tech_dart
		bhvr	:flyer_deploy_ropedeath.bhvr
		bhvroverride
			scale 10 10 10
			Rgb0			60 30 150
			pyrrotate	180 0 0
			positionoffset	0 0 -2.3
			initialvelocityjitter 0 0 0
			physics 0
		end
		lifespan 5
	End


	Event
		ename	drope0
		Type	start
		At 	ropespot0
		bhvr	:flyer_deploy_ropedeath.bhvr
		
	End
	Event
		pother	drope0
		ename	drope1
		Type	start
		At 	ropespot1
		part	:flyer_deploy_cableFade.part
		bhvr	:flyer_deploy_ropedeath.bhvr
		
	End
	Event
		pother	drope1
		ename	drope2
		Type	start
		At 	ropespot2
		part	:flyer_deploy_cableFade.part
		bhvr	:flyer_deploy_ropedeath.bhvr
		
	End
	Event
		pother	drope2
		ename	drope3
		Type	start
		At 	ropespot3
		part	:flyer_deploy_cableFade.part
		bhvr	:flyer_deploy_ropedeath.bhvr
		
	End
	Event
		pother	drope3
		ename	drope4
		Type	start
		At 	ropespot4
		part	:flyer_deploy_cableFade.part
		bhvr	:flyer_deploy_ropedeath.bhvr
		
	End
	Event
		pother	drope4
		ename	drope5
		Type	start
		At 	ropespot5
		part	:flyer_deploy_cableFade.part
		bhvr	:flyer_deploy_ropedeath.bhvr
		
	End
	Event
		pother	drope5
		ename	drope6
		Type	start
		At 	ropespot6
		part	:flyer_deploy_cableFade.part
		bhvr	:flyer_deploy_ropedeath.bhvr
		
	End
	Event
		pother	drope6
		ename	drope7
		Type	start
		At 	ropespot7
		part	:flyer_deploy_cableFade.part
		bhvr	:flyer_deploy_ropedeath.bhvr
		
	End
	Event
		pother	drope7
		ename	drope8
		Type	start
		At 	ropespot8
		part	:flyer_deploy_cableFade.part
		bhvr	:flyer_deploy_ropedeath.bhvr
		
	End
	Event
		pother	drope8
		ename	drope9
		Type	start
		At 	ropespot9
		part	:flyer_deploy_cableFade.part
		bhvr	:flyer_deploy_ropedeath.bhvr
		
	End
	Event
		pother	drope9
		ename	drope10
		Type	start
		At 	ropespot10
		part	:flyer_deploy_cableFade.part
		bhvr	:flyer_deploy_ropedeath.bhvr
		
	End
	Event
		pother	drope10
		ename	drope11
		Type	start
		At 	prime
		part	:flyer_deploy_cableFade.part
		bhvr	:flyer_deploy_ropedeath.bhvr
		
	End

end
