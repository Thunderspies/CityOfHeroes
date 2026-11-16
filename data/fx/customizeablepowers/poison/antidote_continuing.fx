#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

#########################################################

Condition

	Event
		Type	local
		At	head
		
		Part	CustomizeablePowers\Poison\BubblesTrailCont.part
		
		Part	CustomizeablePowers\Poison\PoisonTrailCont.part
		Part	CustomizeablePowers\Poison\PoisonTrailCont2.part
		Part	CustomizeablePowers\Poison\DarkPoisonTrailCont.part
		Part	CustomizeablePowers\Poison\DarkPoisonTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Sound Antidote1_loop 70 70 .33
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 200
	End
	
	Event
		Type	local
		At	chest
		
		Part	CustomizeablePowers\Poison\ElipseCont.part


	End
		
#	Event
#		Type	local
#		At	FootL
#		
#		Part	:BubblesTrailCont.part
#		Part	:PoisonTrailContFeetDark.part
#		
#		Part	:PoisonTrailContFeet.part
#
#
#	End
#	
#	Event
#		Type	local
#		At	FootR
#		
#		Part	:BubblesTrailCont.part
#		Part	:PoisonTrailContFeetDark.part
#
#		Part	:PoisonTrailContFeet.part
#
#	End
End

End				