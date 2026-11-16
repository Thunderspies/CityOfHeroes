#############################################################
## GhostHoldHit.fx
#############################################################

FxInfo

LifeSpan	20

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition

	Event
		Type	posit
		At	root

		part	:GhostGreenMistTrail2Flat.part
		part	:GhostGreenMistTrailFlat.part
	End

	Event
		EName 	Orb
		Type	Local
		At	Root
		part1	:GhostGreenMistTrail.part
		part2	:GhostGreenMistTrail2.part
		Sound miasma_loop 100 100 .5


	End

	Event
		EName 	Orb2
		Type	Local
		At	Root
		part	:GhostGreenMistTrail.part
		part	:GhostGreenMistTrail2.part
		part	:GhostGreenTendrils.part
	End


End

End