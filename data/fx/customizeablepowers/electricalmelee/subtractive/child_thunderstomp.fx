#############################################################
## Thunder_Stomp.fx
#############################################################

FxInfo

Lifespan 100

########################################################

Condition
	On 	time
	Time 	0

	Event
		Type	local
		At	root
		Part	:Thunder_Stomp_rings.part
	End

	Event
		Type	local
		At	footL
		ChildFX	:Child_ThunderStompBranches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		ChildFX	:Child_ThunderStompBranches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		ChildFX	:Child_ThunderStompBranches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		ChildFX	:Child_ThunderStompBranches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		ChildFX	:Child_ThunderStompBranches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		ChildFX	:Child_ThunderStompBranches.fx
		lifespan 100
	End
End

#############################################################

Condition
	On 	time
	Time 	1

	Event
		Type	local
		At	root
		Bhvr	Behaviors\SSLSShake.bhvr
		Part	:Thunder_Stomp_rings.part
		Sound lightningbolt4 120 120 .9
	End

	Event
		Type	local
		At	footL
		ChildFX	:Child_ThunderStompBranches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		ChildFX	:Child_ThunderStompBranches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		ChildFX	:Child_ThunderStompBranches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		ChildFX	:Child_ThunderStompBranches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		ChildFX	:Child_ThunderStompBranches.fx
		lifespan 100
	End

	Event
		Type	local
		At	footL
		ChildFX	:Child_ThunderStompBranches.fx
		lifespan 100
	End
End

#############################################################

End