#############################################################
## IceContinuing.fx
#############################################################

FxInfo
Flags	IsArmor

#########################################################

Input
	Inpname	Hips
End

#########################################################


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
		bhvr	behaviors/soundFade3.bhvr
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
		bhvr	behaviors/soundFade3.bhvr
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

		BHVR	Powers/EarthControl/DamageDrone.bhvr


	End

	Event
		Ename	DroneSpin
		Type	local
		At	Drone

		BHVR	:ChestIce.bhvr
		Part	:Frost.part
		Part	:ColdArmMist.part
		Part	:ColdArmMist2.part
		Part	:SnowFall.part
		Part	:IceChunks.part
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

		BHVR	Powers/EarthControl/DamageDroneOpposite2.bhvr


	End

	Event
		Ename	DroneSpin2
		Type	local
		At	Drone2

		BHVR	:IceOpposite.bhvr
		Part	:Frost.part
		Part	:ColdArmMist.part
		Part	:ColdArmMist2.part
		Part	:SnowFall.part
		Part	:IceChunks.part
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

		BHVR	Powers/EarthControl/1DamageDroneSpinOppositeSML.bhvr


	End

	Event
		Ename	DroneSpin3
		Type	local
		At	Drone3

		BHVR	:IceOppositeSML.bhvr
		Part	:Frost.part
		Part	:ColdArmMist.part
		Part	:ColdArmMist2.part
		Part	:SnowFall.part
		Part	:IceChunks.part
		geom	FX_IceArmor_HEAD

	End


End

End
