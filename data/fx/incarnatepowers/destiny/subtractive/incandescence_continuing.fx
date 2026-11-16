#############################################################
## Incandescence_Continuing.fx
#############################################################

FxInfo
#Lifespan 0
#############################################################
Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Incandescence_Continuing_Crackle.part
		Part	:Incandescence_Continuing_Blob.part
		Part	:Incandescence_Continuing_LightBlob.part
		Part	:Incandescence_Continuing_Streaks.part
	End
End
##############################################################
End