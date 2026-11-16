#########################################################
##	forceBubble
########################################################
FxInfo

########################################################


#Flag	Kickstart


Condition
	On 	Time
	Time 	0

	Event
		
		Type	local
		At	Head

		BhvrOverride
			PositionOffset	0 -1.5 0
		End

		BHVR	Behaviors/GenericParticleFade.bhvr
		
#		Part	:LightBeam.part
#		Part	:LightBeams.part
#		Part	:LightBeamRight.part
#		Part	:LightBeamLeft.part
#		Part	:LightBeamLeft2.part
#		Part	:LightBeamRight2.part
#
		Part	:InspirationYellow.part
		Part	:InspirationBlue.part
		Part	:InspirationRed.part
		Part	:InspirationGreen.part
		Part	:InspirationPurple.part
#	
#		Part	:ElectricityArch.part
#		Part	:ElectricityArch2.part

		Part	:InpirationRings.part
	End


End

End
