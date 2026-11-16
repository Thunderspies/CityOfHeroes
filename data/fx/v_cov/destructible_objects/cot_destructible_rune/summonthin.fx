#########################################################
##	chill touch hit
FxInfo

##################################

#Condition
#	
#	Event
#		EName	Ring
#		Type	Local
#		At	Root
#		part1	Powers/Ability/IntelligenceRingUp.part
#		sound	heal1 80 20 .8
#		
#	End
#
#End


Condition

	Event
		EName	AntiMatter
		Type	local
		At	Root
		
		BhvrOverride	
			PositionOffset	0 .5 0
			PYRrotate	0 -90 0
		End

#		Part	World\Lairs\Portals\PortalRings.part
		Part	World\Lairs\Portals\PortalSpecks.part
#		Part	World\Lairs\Portals\ElectricitySparks.part	
		Part	World\Lairs\Portals\ElectricitySparks2.part
#		Part	World\Lairs\Portals\RiktiPortalRing.part
		Part	World\Lairs\Portals\RiktiPortalRing2.part
		Part	World\Lairs\Portals\PortalThicknessNarrower2.part
#		Sound	eleccage_loop 60 50 .17
	End

End

Condition

	Event
		EName	Ring2
		Type	local
		At	Root
		
		BhvrOverride	
			PositionOffset	0 .5 0
			PYRrotate	0 -90 0
		End

#		Part	World\Lairs\Portals\PortalRingPulse.part
#		Part	World\Lairs\Portals\PortalRingPulse2.part
#		Part	World\Lairs\Portals\PortalOuterRing.part
	End

End

End				