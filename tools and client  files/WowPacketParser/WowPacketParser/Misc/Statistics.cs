﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using WowPacketParser.Enums;

namespace WowPacketParser.Misc
{
    /// <summary>
    /// Represents parsing statistics for a single sniff/file.
    /// </summary>
    public class Statistics
    {
        private readonly int _totalPacketCount;

        private int _sucessPacketCount;
        private int _withErrorsPacketCount;
        private int _notParsedPacketCount;

        private DateTime _startTime;
        private DateTime _endTime;

        /// <summary>
        /// Initialized a new instance of Statistics.
        /// </summary>
        public Statistics()
        {
        }

        /// <summary>
        /// Initializes a new instance of Statistics.
        /// </summary>
        /// <param name="packetCount">The expected total number of packets. Must be higher than zero.
        ///  Throws ArgumentOutOfRangeException if this is not higher than zero.</param>
        public Statistics(int packetCount)
        {
            if (packetCount <= 0)
                throw new ArgumentOutOfRangeException("packetCount", packetCount, "Packet count must be higher than zero.");

            _totalPacketCount = packetCount;
        }

        /// <summary>
        /// Expected total number of packets.
        /// </summary>
        public int TotalPacketCount
        {
            get { return _totalPacketCount; }
        }

        /// <summary>
        /// The total number of packets taken into account in calculations.
        /// </summary>
        public int CalculatedTotalPacketCount
        {
            get { return _sucessPacketCount + _withErrorsPacketCount + _notParsedPacketCount; }
        }

        /// <summary>
        /// Number of packets sucessfully parsed.
        /// </summary>
        public int SucessPacketCount
        {
            get { return _sucessPacketCount; }
        }

        /// <summary>
        /// Number of packets parsed with errors.
        /// </summary>
        public int WithErrorsPacketCount
        {
            get { return _withErrorsPacketCount; }
        }

        /// <summary>
        /// Number of packets that were skipped.
        /// </summary>
        public int NotParsedPacketCount
        {
            get { return _notParsedPacketCount; }
        }

        /// <summary>
        /// Increments by one the number of packets sucessfully parsed.
        /// </summary>
        public void AddSucess()
        {
            Interlocked.Increment(ref _sucessPacketCount);
        }

        /// <summary>
        /// Increments by one the number of packets parsed with errors.
        /// </summary>
        public void AddWithErrors()
        {
            Interlocked.Increment(ref _withErrorsPacketCount);
        }

        /// <summary>
        /// Increments by one the number of packets skipped.
        /// </summary>
        public void AddNotParsed()
        {
            Interlocked.Increment(ref _notParsedPacketCount);
        }

        /// <summary>
        /// Increment respective count
        /// </summary>
        /// <param name="status">Parsing status</param>
        public void AddByStatus(ParsedStatus status)
        {
            switch (status)
            {
                case ParsedStatus.None:
                    break;
                case ParsedStatus.Success:
                    AddSucess();
                    break;
                case ParsedStatus.WithErrors:
                    AddWithErrors();
                    break;
                case ParsedStatus.NotParsed:
                    AddNotParsed();
                    break;
            }
        }

        /// <summary>
        /// Sucessfully parsed packets percentage. [0-100]
        /// </summary>
        /// <returns></returns>
        public double GetSucessPercentage()
        {
            return 100.0 * SucessPacketCount / CalculatedTotalPacketCount;
        }

        /// <summary>
        /// Parsed packets with errors percentage. [0-100]
        /// </summary>
        /// <returns></returns>
        public double GetWithErrorsPercentage()
        {
            return 100.0 * WithErrorsPacketCount / CalculatedTotalPacketCount;
        }

        /// <summary>
        /// Skipped packets percentage. [0-100]
        /// </summary>
        /// <returns></returns>
        public double GetNotParsedPercentage()
        {
            return 100.0 * NotParsedPacketCount / CalculatedTotalPacketCount;
        }

        /// <summary>
        /// Sets the time parsing started.
        /// </summary>
        /// <param name="startTime">Parsing started</param>
        public void SetStartTime(DateTime startTime)
        {
            _startTime = startTime;
        }

        /// <summary>
        /// Sets the time parsing ended.
        /// </summary>
        /// <param name="endTime">Parsing ended</param>
        public void SetEndTime(DateTime endTime)
        {
            _endTime = endTime;
        }

        /// <summary>
        /// The time it took to parse
        /// </summary>
        /// <returns>Time interval TimeSpan</returns>
        public TimeSpan GetParsingTime()
        {
            return _endTime - _startTime;
        }

        /// <summary>
        /// Formatted string that includes statistic information
        ///  (total number of packets, parsing time, parsing percentages, ...)
        /// </summary>
        /// <returns>String</returns>
        public override string ToString()
        {
            var parsingTime = GetParsingTime();

            var sb = new StringBuilder("Parsed ")
                .Append(SucessPacketCount)
                .Append(" (").AppendFormat("{0:F3}", GetSucessPercentage()).Append("%) ")
                .Append("packets sucessfully, ")
                .Append(WithErrorsPacketCount)
                .Append(" (").AppendFormat("{0:F3}", GetWithErrorsPercentage()).Append("%) ")
                .Append("with errors and skipped ")
                .Append(NotParsedPacketCount)
                .Append(" (").AppendFormat("{0:F3}", GetNotParsedPercentage()).Append("%) ")
                .Append(" (total: ").Append(CalculatedTotalPacketCount).Append(")");

            if (parsingTime.Milliseconds != 0)
                sb.Append(" in ").Append(parsingTime.ToFormattedString());

            return sb.ToString();
        }

        /// <summary>
        /// Goes through each packet in a collection of packets and returns an instance of Statistics
        ///  containing parsing status information.
        /// </summary>
        /// <param name="packets">Collection of packets</param>
        /// <returns>An instance of Statistics</returns>
        public static Statistics BuildStats(ICollection<Packet> packets)
        {
            var stats = new Statistics(packets.Count);

            Parallel.ForEach(packets, packet =>
            {
                if (!packet.WriteToFile)
                    stats.AddNotParsed();
                else
                {
                    switch (packet.Status)
                    {
                        case ParsedStatus.Success:
                            stats.AddSucess();
                            break;
                        case ParsedStatus.WithErrors:
                            stats.AddWithErrors();
                            break;
                        case ParsedStatus.NotParsed:
                            stats.AddNotParsed();
                            break;
                    }
                }
            });

            return stats;
        }
    }
}
