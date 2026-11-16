#############################################################
## Fighter_Jet.fx
#############################################################

FxInfo

#############################################################


Condition
	On	Time
	Time	0

	Event
		Ename	Contrail
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	3 2 -10
		End
		Part	:JetTrail.part
	End


	Event
		Ename	Contrail
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-3 2 -10
		End
		Part	:JetTrail.part
	End
End

#############################################################

End