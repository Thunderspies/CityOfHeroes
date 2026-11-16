#########################################################
##	
##
FxInfo

#Flags	IsArmor


Flags    InheritAlpha

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
	End
End

Condition
	On	Time
	Time 	8
		
	Event
		Type	Local
		At 	origin
		Sound glow6_loop 100 100 .55
	End
End

#####################################################################################

Condition
	On 	cycle
	Time 	10
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	head
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	
End

####################################################################################################

Condition
	
	On	Time
	Time	30

	Event
		Ename	Drone
		Type	posit
		At	Root
		
		BHVR	:DamageDrone.bhvr
		
				
	End

	Event
		Ename	DroneSpin
		Type	local
		At	Drone
		
		BHVR	:DamageDroneSpin.bhvr
		Part	:MineralPsionicRingsFlat.part		
		Part	:MineralPsionicRings.part	
		geom	rock
				
	End


End


Condition
	
	On	Time
	Time	25

	Event
		Ename	Drone2
		Type	posit
		At	Root
		
		BHVR	:DamageDroneOpposite.bhvr
		
				
	End

	Event
		Ename	DroneSpin2
		Type	local
		At	Drone2
		
		BHVR	:DamageDroneSpinOpposite.bhvr
		Part	:MineralPsionicRingsFlat.part		
		Part	:MineralPsionicRings.part
		geom	rock
				
	End
		
End


Condition
	
	On	Time
	Time	45
	
	Event
		Ename	Drone3
		Type	posit
		At	Root
		
		BHVR	:DamageDroneOppositeSML.bhvr
		
				
	End

	Event
		Ename	DroneSpin3
		Type	local
		At	Drone3
		
		BHVR	:DamageDroneSpinOppositeSML.bhvr
		Part	:MineralPsionicRingsFlat.part		
		Part	:MineralPsionicRings.part
		geom	rock
				
	End
	

End

End
