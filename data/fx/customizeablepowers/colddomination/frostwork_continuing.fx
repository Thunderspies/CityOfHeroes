#########################################################
##	
##
FxInfo

Flags	IsArmor

#####################################################################################
##Lens Flares
#####################################################


Condition
	On	Time
	Time 	0
		
	Event
		Type	Local
		At 	origin
		Sound Stone4 100 100 .96
	End
End

Condition
	On	Time
	Time 	8
		
	Event
		Type	Local
		At 	origin
		Sound Mineral_loop 100 100 .72
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 300
	End
End

Condition
	On	Time
	Time 	8
		
	Event
		Type	Local
		At 	origin
		Sound glow6_loop 100 100 .55
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 300
	End
End

####################################################################################################

Condition
	
	Event
		Ename	Drone
		Type	posit
		At	Root
		
		BHVR	CustomizeablePowers\ColdDomination\DamageDrone.bhvr
		
				
	End

	Event
		Ename	DroneSpin
		Type	local
		At	Drone
		
		BHVR	CustomizeablePowers\ColdDomination\ChestIce.bhvr
		Part	CustomizeablePowers\ColdDomination\Frost.part
		Part	CustomizeablePowers\ColdDomination\ColdArmMist.part
		Part	CustomizeablePowers\ColdDomination\ColdArmMist2.part
		Part	CustomizeablePowers\ColdDomination\SnowFall.part
		Part	CustomizeablePowers\ColdDomination\IceChunks.part
		geom	FX_IceArmor_HEAD
				
	End


End


Condition
	
	On	Time
	Time	2

	Event
		Ename	Drone2
		Type	posit
		At	Root
		
		BHVR	CustomizeablePowers\ColdDomination\DamageDroneOpposite2.bhvr
		
				
	End

	Event
		Ename	DroneSpin2
		Type	local
		At	Drone2
		
		BHVR	CustomizeablePowers\ColdDomination\IceOpposite.bhvr
		Part	CustomizeablePowers\ColdDomination\Frost.part
		Part	CustomizeablePowers\ColdDomination\ColdArmMist.part
		Part	CustomizeablePowers\ColdDomination\ColdArmMist2.part
		Part	CustomizeablePowers\ColdDomination\SnowFall.part
		Part	CustomizeablePowers\ColdDomination\IceChunks.part
		geom	FX_IceArmor_HEAD
				
	End
		
End


Condition
	
	On	Time
	Time	5
	
	Event
		Ename	Drone3
		Type	posit
		At	Root
		
		BHVR	CustomizeablePowers\ColdDomination\1DamageDroneSpinOppositeSML.bhvr
		
				
	End

	Event
		Ename	DroneSpin3
		Type	local
		At	Drone3
		
		BHVR	CustomizeablePowers\ColdDomination\IceOppositeSML.bhvr
		Part	CustomizeablePowers\ColdDomination\Frost.part
		Part	CustomizeablePowers\ColdDomination\ColdArmMist.part
		Part	CustomizeablePowers\ColdDomination\ColdArmMist2.part
		Part	CustomizeablePowers\ColdDomination\SnowFall.part
		Part	CustomizeablePowers\ColdDomination\IceChunks.part
		geom	FX_IceArmor_HEAD
				
	End
	

End

End
