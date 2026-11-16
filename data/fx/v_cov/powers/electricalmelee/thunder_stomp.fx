#############################################################
## Thunder_Stomp.fx
#############################################################

FxInfo

Lifespan 100

########################################################

Condition
	On 	time
	Time 	1


	Event
		Type	local
		At	root
		Part	ENEMYFX\V_Diable_Demons\Thunder_Stomp_rings.part
		Bhvr	Behaviors\SSLSShake.bhvr
		Sound lightningbolt4 120 120 .9
	End

	Event
		Type	local
		At	footL
		childFX	ENEMYFX\V_Diable_Demons\Thunder_Stomp_Branches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		childFX	ENEMYFX\V_Diable_Demons\Thunder_Stomp_Branches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		childFX	ENEMYFX\V_Diable_Demons\Thunder_Stomp_Branches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		childFX	ENEMYFX\V_Diable_Demons\Thunder_Stomp_Branches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		childFX	ENEMYFX\V_Diable_Demons\Thunder_Stomp_Branches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		childFX	ENEMYFX\V_Diable_Demons\Thunder_Stomp_Branches.fx
		lifespan 100
	End
End

Condition
	On 	time
	Time 	0

	Event
		Type	local
		At	root
		Part	ENEMYFX\V_Diable_Demons\Thunder_Stomp_rings.part
	End

	Event
		Type	local
		At	footL
		childFX	ENEMYFX\V_Diable_Demons\Thunder_Stomp_Branches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		childFX	ENEMYFX\V_Diable_Demons\Thunder_Stomp_Branches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		childFX	ENEMYFX\V_Diable_Demons\Thunder_Stomp_Branches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		childFX	ENEMYFX\V_Diable_Demons\Thunder_Stomp_Branches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		childFX	ENEMYFX\V_Diable_Demons\Thunder_Stomp_Branches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		childFX	ENEMYFX\V_Diable_Demons\Thunder_Stomp_Branches.fx
		lifespan 100
	End
End

#############################################################

End