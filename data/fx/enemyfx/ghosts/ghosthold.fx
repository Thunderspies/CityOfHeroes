#############################################################
## GhostHold.fx
#############################################################

FxInfo

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
		part1	:GhostGreenMistHold.part
		part2	:GhostGreenMistHold2.part
		Part	:GhostGreenMistHold3.part
		Sound miasma_loop 100 100 .5


	End

	Event
		EName 	Orb2
		Type	Local
		At	Root
		part	:GhostGreenMistHold.part
		part	:GhostGreenMistHold2.part
#		Part	:GhostGreenMistHold3.part
		part	:GhostGreenTendrilsHold.part
	End


End

End