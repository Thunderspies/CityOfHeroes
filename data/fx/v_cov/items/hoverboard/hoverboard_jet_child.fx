#############################################################
## Hoverboard_Jet_Child.fx
#############################################################

FxInfo
Flags InheritAlpha DontSuppress
###############################################################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Root
	#	Part	:Contrail.part
		Bhvr	Behaviors\GenericParticleFade.bhvr

		#Part	:AirJet_Long.part
		#Part	:AirJet.part
	End

	Event
		Type	Local
		At	Origin
		Bhvr	Behaviors\GenericParticleFade.bhvr
		#Bhvr	V_COV\Items\Hoverboard\FastFade.bhvr
		BhvrOverride
			PyrRotate	0 0 0
		End
		Part	:AirJet_Flare.part
		Part	:AirJet.part
	End
End
################################################################

End
