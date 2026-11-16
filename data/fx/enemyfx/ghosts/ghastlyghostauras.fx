########################################################
##	Fire_Ball
##
FxInfo

Flags    InheritAlpha

##################################



Condition
	On	Time
	Time	60
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:GhostMist.part
		Part	:GhostMistTendrils.part
		Part	:GhostMistAdditive.part
		Sound forcefield5_loop 60 60 .7
	

	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:GhostMist.part
		Part	:GhostMistTendrils.part
		Part	:GhostMistAdditive.part
		Sound miasma_loop 60 60 .77
	

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:GhostMist.part
		Part	:GhostMistTendrils.part
		Part	:GhostMistAdditive.part


	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:GhostMist.part
		Part	:GhostMistTendrils.part
		Part	:GhostMistAdditive.part

	
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:GhostMist.part
		Part	:GhostMistTendrils.part
		Part	:GhostMistAdditive.part

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:GhostMist.part
		Part	:GhostMistTendrils.part
		Part	:GhostMistAdditive.part
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:GhostMist.part
		Part	:GhostMistTendrils.part
		Part	:GhostMistAdditive.part
		
	End

		
##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:GhostMist.part
		Part	:GhostMistTendrils.part
		Part	:GhostMistAdditive.part

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:GhostMist.part
		Part	:GhostMistTendrils.part
		Part	:GhostMistAdditive.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:GhostMist.part
		Part	:GhostMistTendrils.part
		Part	:GhostMistAdditive.part

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:GhostMist.part
		Part	:GhostMistTendrils.part
		Part	:GhostMistAdditive.part
	End

End    


End	

