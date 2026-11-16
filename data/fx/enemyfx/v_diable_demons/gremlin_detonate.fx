#########################################################
##	General red electricity crawling across characters limbs
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	HandR
End

LifeSpan	100

########################################################



Condition

	On 	time
	Time 	50

	Event
		Ename   groundwave
		Type	local
		At	root
		bhvr	:Gremlin_det_Shockwave.bhvr

		lifespan 15
	End	

	Event
		Type	local
	
		At	root
		Part1	:Gremlin_det_rings.part
	End
end


Condition

	On 	time
	Time 	49
	Event
		Type	local
	
		At	root
		Part1	:Gremlin_det_rings.part
	End
end

Condition

	On 	time
	Time 	51
	Event
		Type	local
	
		At	root
		Part1	:Gremlin_det_rings.part
	End
end


Condition

	On 	cycle
	Time 	1

	Event
		Type	start
		At	groundwave
		part	:Gremlin_det_Shockwave.part
		part	:Gremlin_det_Shockwave1.part

		lifespan 15
	End	
End


#Condition
#	On	Time
#	Time	0
#	
#	Event
#		EName	HandGlow1
#		Type	Local
#		At	Chest
#		Part1	:Gremlin_Det_glow.part
##		Part1	POWERS\ElectricityControl\ElectricityGlow.part
##		Part2	POWERS\ElectricityControl\ElectricityGlowStar.part
##		Part3	POWERS\ElectricityControl\ElectricityArch4a.part
##		Part4	POWERS\ElectricityControl\ElectricityArchSmalla.part
##		Sound	elecballhit 80 30 .8
#	End
#
#	Event
#		EName	continuingArchs
#		Type	Local
#		At	Chest
#		Part1	POWERS\ElectricityControl\ElectricityArchContinuing.part
#		Part2	POWERS\ElectricityControl\ElectricityArchContinuing2.part
#	End
#End



Condition

	On 	time
	Time 	50

	Event
		Ename   ShockwaveBall

		At	chest
		bhvr	:Gremlin_det_Splode.bhvr
		Geom    EarthGraspHit
		Type	local
		lifespan 5
	End	
End



Condition

	On 	time
	Time 	10

	Event
		Ename   ShockwaveBall

		At	groundzero
		part	:Gremlin_det_Chargers.part
		part	:Gremlin_det_ChargersFat.part
		part	:Gremlin_det_Chargers.part
		part	:Gremlin_det_ChargersFat.part
		Type	local
		lifespan 45
	End	
End



Condition

	On 	time
	Time 	20

	Event
		Ename   ShockwaveBall

		At	groundzero
		part	:Gremlin_det_Chargers.part
		part	:Gremlin_det_ChargersFat.part
		part	:Gremlin_det_Chargers.part
		part	:Gremlin_det_ChargersFat.part
		Type	local
		lifespan 35
	End	
End



Condition

	On 	time
	Time 	30

	Event
		Ename   ShockwaveBall

		At	chest
		part	:Gremlin_det_Chargers.part
		part	:Gremlin_det_ChargersFat.part
		part	:Gremlin_det_Chargers.part
		part	:Gremlin_det_ChargersFat.part
		Type	local
		lifespan 25
	End	
End


Condition

	On 	time
	Time 	0

	Event
		Ename	groundzero
		Type	local
	
		At	chest
		bhvr	:Gremlin_det_Swell.bhvr
		part	:Gremlin_det_Chargers.part
		part	:Gremlin_det_ChargersFat.part
		lifespan 50
#		geom    Testing_Target
		pother  chest
	End


	Event
		Ename	lift
		Type	local
	
		At	root
		bhvr	:Gremlin_det_grow.bhvr
		
		lifespan 65
#		geom    Testing_Target
	End

	Event
		EName	HandGlow1
		Type	Local
		At	chest
		Part1	:Gremlin_Det_glow.part
#		Part1	POWERS\ElectricityControl\ElectricityGlow.part
#		Part2	POWERS\ElectricityControl\ElectricityGlowStar.part
#		Part3	POWERS\ElectricityControl\ElectricityArch4a.part
#		Part4	POWERS\ElectricityControl\ElectricityArchSmalla.part
#		Sound	elecballhit 80 30 .8
		lifespan 51
	End
	Event
		Type	Local
		At	hips
		Part1	:Gremlin_Det_glow.part
	End

end


Condition

	On 	cycle
	Time 	1

	Event
		Type	local  
		At	lift
		childfx	:Gremlin_det_Branches.fx
		lifespan 5
	end

end

Condition

	On 	cycle
	Time 	2

	Event
		Type	local  
		At	lift
		childfx	:Gremlin_det_Branches.fx
		lifespan 5
	end

end

Condition

	On 	cycle
	Time 	3

	Event
		Type	local  
		At	lift
		childfx	:Gremlin_det_Branches.fx
		lifespan 5
	end

end
######################################
