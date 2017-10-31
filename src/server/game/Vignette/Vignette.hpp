////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef VIGNETTE_H
#define VIGNETTE_H

#include "Common.h"
#include "Object.h"
#include "DB2Stores.h"
#include "GameObject.h"
#include "Creature.h"

namespace Vignette
{
    /// This enum is internal only (not sended or related to client-side)
    /// It's represent type of vignette to know how we must handle them
    enum class Type : uint8
    {
        SourceCreature,           ///< Used for vignette linked to creature, player show the vignette if the creature is in the view-range
        SourceGameObject,         ///< Used for vignette linked to gameobject, player show the vignette if the gameobject is in the view-range
        SourceRare,               ///< Used for vignette linked to rare creature, player show the vignette if the creature is in the view-range and if the player doesn't have the tracking quest
        SourceTreasure,           ///< Used for vignette linked to treasure gameobject, player show the vignette if the treasure is in the view-range and if the player doesn't have the tracking quest
        SourceScript              ///< Used for script, that kind of vignette are never remove automaticaly
    };

    /************************************************************************/
    /*                         HELPERS                                      */
    /************************************************************************/

    /**
    * Return VignetteEntry (db2 entry) from WorldObject if available
    * Work only for Creature and Gameobject
    * @param p_Target : WorldObject for which we wanna get the vignette entry
    */
    inline VignetteEntry const* GetVignetteEntryFromWorldObject(WorldObject const* p_Target) ///<  Unused function 'GetVignetteEntryFromWorldObject'
    {
        uint32 l_VignetteId = 0;

        if (p_Target->GetTypeId() == TYPEID_UNIT)
            l_VignetteId = p_Target->ToCreature()->GetCreatureTemplate()->VignetteID;
        if (p_Target->GetTypeId() == TYPEID_GAMEOBJECT)
            l_VignetteId = p_Target->ToGameObject()->GetGOInfo()->GetVignetteId();

        if (l_VignetteId == 0)
            return nullptr;

        return sVignetteStore.LookupEntry(l_VignetteId);
    }

    /**
    * Return TrackingQuest (db entry) from WorldObject if available
    * Work only for Creature and Gameobject
    * @param p_Target : WorldObject for which we wanna get the tracking quest
    */
    inline uint32 GetTrackingQuestIdFromWorldObject(WorldObject const* p_Target) ///<  Unused function 'GetTrackingQuestIdFromWorldObject'
    {
        uint32 l_TrackingQuest = 0;

        if (p_Target->GetTypeId() == TYPEID_UNIT)
            l_TrackingQuest = p_Target->ToCreature()->GetCreatureTemplate()->TrackingQuestID;
        if (p_Target->GetTypeId() == TYPEID_GAMEOBJECT)
            l_TrackingQuest = p_Target->ToGameObject()->GetGOInfo()->GetTrackingQuestId();

        return l_TrackingQuest;
    }

    /**
    * Return default vignette type (@see enum Vignette::Type) from WorldObject
    * Work only for Creature and Gameobject
    * @param p_Target : WorldObject for which we wanna get the default vignette type
    */
    inline Vignette::Type GetDefaultVignetteTypeFromWorldObject(WorldObject const* p_Target) ///<  Unused function 'GetDefaultVignetteTypeFromWorldObject'
    {
        switch (p_Target->GetTypeId())
        {
            case TYPEID_UNIT:
                return Vignette::Type::SourceCreature;
            case TYPEID_GAMEOBJECT:
                return Vignette::Type::SourceGameObject;
            /// Can't happen
            default:
                return Vignette::Type::SourceScript;
        }
    }

    /**
    * Return tracking vignette type (@see enum Vignette::Type) from WorldObject
    * Work only for Creature and Gameobject
    * @param p_Target : WorldObject for which we wanna get the tracking vignette type
    */
    inline Vignette::Type GetTrackingVignetteTypeFromWorldObject(WorldObject const* p_Target) ///< Unused function  GetTrackingVignetteTypeFromWorldObject
    {
        switch (p_Target->GetTypeId())
        {
            case TYPEID_UNIT:
                return Vignette::Type::SourceRare;
            case TYPEID_GAMEOBJECT:
                return Vignette::Type::SourceTreasure;
            /// Can't happen
            default:
                return Vignette::Type::SourceScript;
        }
    }

    /// Entity is the class which represent the vignette
    class Entity
    {
        friend class Manager;

        public:

            /**
            * Update the position of the vignette
            * @param p_NewPosition : New position of the vignette
            */
            void UpdatePosition(G3D::Vector3 p_NewPosition);

            /**
            * Reset the need of client update
            */
            void ResetNeedClientUpdate() { m_NeedClientUpdate = false; }

            /**
            * Get the guid of the vignette
            * @return guid : New position of the vignette
            */
            uint64 GetGuid() const { return m_Guid; }

            /**
            * Get the source guid of the vignette, can be 0
            * @return source guid
            */
            uint64 GeSourceGuid() const { return m_SourceGuid; }

            /**
            * Get the vignette type
            * @return Type @see Vignette::Type
            */
            Type  GetVignetteType() const { return m_Type; }

            /**
            * Check if the vignette need to be update client-side
            * @return result as bool
            */
            bool NeedClientUpdate() const { return m_NeedClientUpdate; }

            /**
            * Get the current position of the vignette
            * @return Vector3
            */
            G3D::Vector3 const& GetPosition() const { return m_Position; }

            /**
            * Get the Vignette Entry (Vignette.db2)
            * @return VignetteEntry
            */
            VignetteEntry const* GetVignetteEntry() const { return m_VignetteEntry; }

        private:


            /**
            * Vignette Entity constructor
            * @param p_VignetteEntry : VignetteEntry, refer to Vignette.db2
            * @param p_MapID : ID of the map where the vignette are, refer to Map.dbc
            */
            Entity(VignetteEntry const* p_VignetteEntry, uint32 const p_MapId);

            /*
            * Vignette Entity destructor
            */
            ~Entity();

            /**
            * Create the vignette
            * @param p_Type : Type of the vignette, @see enum Vignette::Type
            * @param p_Position: World position of the vignette
            * @param p_SourceGuid: Guid of the source (can be gameobject or creature)
            */
            void Create(Type p_Type, G3D::Vector3 p_Position, uint64 p_SourceGuid);

            uint64               m_Guid;                    ///< Guid of the current vignette (GUID_TYPE_VIGNETTE)
            uint64               m_SourceGuid;              ///< Guid of the source if any (can be gameobject or creature)
            uint32 const         m_Map;                     ///< Map Id of the current vignette (we only send vignette which are on the same map the player is)
            VignetteEntry const* m_VignetteEntry;           ///< Vignette data (db2)
            G3D::Vector3         m_Position;                ///< World coordinates of the current vignette, client compute 2D minimap coord itself
            Type                 m_Type;                    ///< Type of the vignette, @see enum Vignette::Type

        protected:

            bool                m_NeedClientUpdate;         ///< If true, the vignette need to be updated client-side
    };
}
#endif
