#########################################################
##	forceBubble
########################################################
FxInfo

########################################################


#Flag	Kickstart



Condition
	On	Time
	Time	0

	Event	
		Type	Local
		At	origin
		Sound CompAmb2_loop 60 60 .85
	End
End

Condition
	On	Time
	Time	0

	Event	
		Type	Local
		At	origin
		Sound glow7_loop 60 60 .9
	End
End

Condition
	On	Time
	Time	0

	Event	
		Type	Local
		At	origin
		Sound computerx7_loop 60 60 .35
	End
End


Condition
	On	Time
	Time	0

	Event	
		Type	Local
		At	origin
		Sound electricityhum4_loop 60 60 .45
	End
End

Condition
	On 	Time
	Time 	0

	Event
		
		Type	local
		At	Head

		BhvrOverride
			PositionOffset	0 1.5 0
		End

		BHVR	Behaviors/GenericParticleFade.bhvr
		
		Part	:TechElectrons.part
		Part	:TechElectronsCore.part

		Part	:TechElectronsCoreRing.part
		Part	:TechElectronsBubble.part

		Part	:TechLightBeam.part
		Part	:TechLightBeams.part
		Part	:TechLightBeamsLong.part
		Part	:TechLightBeamRight.part
		Part	:TechLightBeamLeft.part
		Part	:TechLightBeamLeft2.part
		Part	:TechLightBeamRight2.part

		Part	:TechInspirationYellow.part
		Part	:TechInspirationBlue.part
		Part	:TechInspirationRed.part
		Part	:TechInspirationGreen.part
		Part	:TechInspirationPurple.part
	
	End


End

End
